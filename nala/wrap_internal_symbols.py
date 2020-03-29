import struct


ELFCLASS64 = 2

ELFDATA2LSB = 1

ET_REL = 1

SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4
SHT_REL = 9

STB_GLOBAL = 1

SHN_UNDEF = 0


class Error(Exception):
    pass


class Elf64LsbHeader:

    def __init__(self, data):
        self._data = data
        self.e_type = struct.unpack('<H', data[16:18])[0]
        self.e_shnum = struct.unpack('<H', data[60:62])[0]
        self.e_shoff = struct.unpack('<Q', data[40:48])[0]
        self.e_shentsize = struct.unpack('<H', data[58:60])[0]

    @property
    def data(self):
        self._data[40:48] = struct.pack('<Q', self.e_shoff)

        return self._data


class Elf64LsbSectionHeader:

    def __init__(self, data):
        self._data = data
        self.sh_type = struct.unpack('<I', data[4:8])[0]
        self.sh_offset = struct.unpack('<Q', data[24:32])[0]
        self.sh_size = struct.unpack('<Q', data[32:40])[0]
        self.sh_addralign = struct.unpack('<Q', data[48:56])[0]

    @property
    def data(self):
        self._data[24:40] = struct.pack('<QQ',
                                        self.sh_offset,
                                        self.sh_size)

        return self._data


class Elf64LsbRela:

    def __init__(self, data):
        self._data = data
        self.symbol_index = struct.unpack('<I', data[12:16])[0]

    @property
    def data(self):
        self._data[12:16] = struct.pack('<I', self.symbol_index)

        return self._data


class Section:

    def __init__(self, header, elf):
        self.header = header
        offset = header.sh_offset
        self.data = elf[offset:offset + header.sh_size]


def load_section_headers(elf, e_shnum, e_shoff, e_shentsize):
    section_headers = []

    for i in range(e_shnum):
        offset = e_shoff + i * e_shentsize
        header_data = elf[offset:offset + e_shentsize]
        section_header = Elf64LsbSectionHeader(header_data)
        section_headers.append(section_header)

    return section_headers


def load_elf_header(elf):
    magic = elf[0:4]

    if magic != b'\x7fELF':
        raise Error(f'Invalid ELF header magic {magic}.')

    file_class = elf[4]
    data_encoding = elf[5]

    if file_class == ELFCLASS64 and data_encoding == ELFDATA2LSB:
        header = Elf64LsbHeader(elf[0:64])
    else:
        raise Error('Only little-endian 64-bit files are supported.')

    if header.e_type != ET_REL:
        raise Error('Only relocatable files are supported.')

    return header


def create_undefined_global_symbol(name_offset):
    return struct.pack('<IBBHQQ', name_offset, STB_GLOBAL << 4, 0, SHN_UNDEF, 0, 0)


class Elf64File:

    def __init__(self, elf, symbol_names):
        self._header = load_elf_header(elf)
        section_headers = load_section_headers(elf,
                                               self._header.e_shnum,
                                               self._header.e_shoff,
                                               self._header.e_shentsize)
        self._sections = [
            Section(section_header, elf) for section_header in section_headers
        ]
        self._symtab_section = self._find_section(SHT_SYMTAB)
        self._strtab_section = self._find_section(SHT_STRTAB)
        self._rela_sections = self._find_sections(SHT_RELA)
        self._strtab = {}
        self._symbol_names = []
        self._symbol_name_by_offset = {}
        self._relas = []
        symtab_data = self._symtab_section.data

        for offset in range(0, len(symtab_data), 24):
            binding = (symtab_data[offset + 4] >> 4)

            if binding != STB_GLOBAL:
                continue

            size = struct.unpack('<Q', symtab_data[offset + 16:offset + 24])[0]

            if size == 0:
                continue

            name_offset = struct.unpack('<I', symtab_data[offset:offset + 4])[0]
            name = self._find_string_in_strtab(name_offset)

            if name not in symbol_names:
                continue

            self._symbol_names.append(name)
            self._symbol_name_by_offset[offset] = name

        if not self.has_symbols_to_wrap():
            return

        for rela_section in self._rela_sections:
            for offset in range(0, len(rela_section.data), 24):
                rela = Elf64LsbRela(rela_section.data[offset:offset + 24])
                symbol_offset = 24 * rela.symbol_index

                if symbol_offset not in self._symbol_name_by_offset:
                    continue

                self._relas.append((offset, rela_section, rela))

    def has_symbols_to_wrap(self):
        return bool(self._symbol_name_by_offset)

    def _find_string_in_strtab(self, offset):
        if offset not in self._strtab:
            end = self._strtab_section.data.find(b'\x00', offset)
            name = self._strtab_section.data[offset:end]
            self._strtab[offset] = name.decode('ascii')

        return self._strtab[offset]

    def to_bytes(self):
        """ELF header first, then sections and last section table header.

        """

        offset = 64
        section_headers = []
        section_datas = []

        for section in self._sections:
            alignment = section.header.sh_addralign

            if alignment < 1:
                alignment = 1

            padding_size = (alignment - (offset % alignment))

            if padding_size == alignment:
                padding_size = 0

            section_datas.append(b'\x00' * padding_size)
            offset += padding_size
            section.header.sh_offset = offset
            section.header.sh_size = len(section.data)
            offset += section.header.sh_size
            section_headers.append(section.header.data)
            section_datas.append(section.data)

        self._header.e_shoff = offset

        return b''.join([self._header.data] + section_datas + section_headers)

    def wrap_symbols(self):
        strtab = []
        symtab = []
        symbol_name_to_index = {}
        strtab_offset = len(self._strtab_section.data)
        wrapped_symbol_index = len(self._symtab_section.data) // 24

        for symbol_name in self._symbol_names:
            # Add wrapped string to .strtab.
            strtab_entry = f'__wrap_{symbol_name}'.encode('ascii') + b'\x00'
            strtab.append(strtab_entry)

            # Create an undefiend symbol in .symtab.
            symtab.append(create_undefined_global_symbol(strtab_offset))
            symbol_name_to_index[symbol_name] = wrapped_symbol_index

            strtab_offset += len(strtab_entry)
            wrapped_symbol_index += 1

        self._strtab_section.data += b''.join(strtab)
        self._symtab_section.data += b''.join(symtab)

        # Make all calls call the wrapper instead.
        for offset, rela_section, rela in self._relas:
            symbol_offset = 24 * rela.symbol_index
            symbol_name = self._symbol_name_by_offset[symbol_offset]
            rela.symbol_index = symbol_name_to_index[symbol_name]
            rela_section.data[offset:offset + 24] = rela.data

    def _find_section(self, sh_type):
        for section in self._sections:
            if section.header.sh_type == sh_type:
                return section

        raise Error(f'Section of type {sh_type} not found.')

    def _find_sections(self, sh_type):
        return [
            section
            for section in self._sections
            if section.header.sh_type == sh_type
        ]


def wrap_internal_symbols(object_elf, symbol_names):
    """Wrap given symbols when called internally within an object file, as
    GNU ld only wraps calls between objects (for good reasons). This
    allows mocking function calls within a C source file, given that
    the compiler do not inline those.

    """

    elffile = Elf64File(bytearray(object_elf), set(symbol_names))

    if not elffile.has_symbols_to_wrap():
        return object_elf

    elffile.wrap_symbols()

    return elffile.to_bytes()
