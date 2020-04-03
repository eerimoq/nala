import struct


ELFCLASS64 = 2

ELFDATA2LSB = 1

ET_REL = 1

SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4

STB_GLOBAL = 1

SHN_UNDEF = 0
SHN_LORESERVE = 0xff00

ST_UDEF_GLOB = struct.Struct('<IBBHQQ')


class Error(Exception):
    pass


class Elf64LsbHeader:

    ST_HEADER_1 = struct.Struct('<H22xQ10xHHH')
    ST_HEADER_2 = struct.Struct('<Q')

    def __init__(self, data):
        self._data = data
        (self.e_type,
         self.e_shoff,
         self.e_shentsize,
         self.e_shnum,
         self.e_shstrndx) = self.ST_HEADER_1.unpack_from(data, 16)

        if self.e_shnum == 0:
            raise Error('ToDo: Support many sections.')

        if self.e_shstrndx == SHN_UNDEF:
            raise Error('Missing section header string table.')

        if self.e_shstrndx >= SHN_LORESERVE:
            raise Error('ToDo: Support many sections.')

        self.shstrtab_index = self.e_shstrndx

    @property
    def data(self):
        self.ST_HEADER_2.pack_into(self._data, 40, self.e_shoff)

        return self._data


class Elf64LsbSectionHeader:

    ST_HEADER_1 = struct.Struct('<II16xQQ8xQ8x')
    ST_HEADER_2 = struct.Struct('<QQ')

    def __init__(self, data):
        self._data = data
        (self.sh_name,
         self.sh_type,
         self.sh_offset,
         self.sh_size,
         self.sh_addralign) = self.ST_HEADER_1.unpack_from(data)

    @property
    def data(self):
        self.ST_HEADER_2.pack_into(self._data, 24, self.sh_offset, self.sh_size)

        return self._data


class Elf64LsbRela:

    ST_HEADER = struct.Struct('<I')

    def __init__(self, data):
        self._data = data
        self.symbol_index = self.ST_HEADER.unpack_from(data, 12)[0]

    @property
    def data(self):
        self.ST_HEADER.pack_into(self._data, 12, self.symbol_index)

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
    return ST_UDEF_GLOB.pack(name_offset, STB_GLOBAL << 4, 0, SHN_UNDEF, 0, 0)


class Elf64File:

    ST_SYMTAB = struct.Struct('<I12xQ')

    def __init__(self, elf, symbol_names):
        self._header = load_elf_header(elf)
        section_headers = load_section_headers(elf,
                                               self._header.e_shnum,
                                               self._header.e_shoff,
                                               self._header.e_shentsize)
        self._sections = [
            Section(section_header, elf) for section_header in section_headers
        ]
        self._shstrtab_section = self._sections[self._header.shstrtab_index]
        self._shstrtab = {}
        self._symtab_section = self._find_section_by_name('.symtab')
        self._strtab_section = self._find_section_by_name('.strtab')
        self._rela_sections = self._find_sections_by_type(SHT_RELA)
        self._strtab = {}
        self._symbol_names = []
        self._symbol_name_by_offset = {}
        self._relas = []
        self._load_symbols(symbol_names)

        if self.has_symbols_to_wrap():
            self._load_rela()

    def has_symbols_to_wrap(self):
        return bool(self._symbol_name_by_offset)

    def wrap_symbols(self):
        strtab = []
        symtab = []
        symbol_name_to_wrapped_index = {}
        strtab_offset = len(self._strtab_section.data)
        wrapped_symbol_index = len(self._symtab_section.data) // 24

        for symbol_name in self._symbol_names:
            # Add wrapped string to .strtab.
            strtab_entry = f'__wrap_{symbol_name}\x00'.encode('utf-8')
            strtab.append(strtab_entry)

            # Create an undefiend symbol in .symtab.
            symtab.append(create_undefined_global_symbol(strtab_offset))
            symbol_name_to_wrapped_index[symbol_name] = wrapped_symbol_index

            strtab_offset += len(strtab_entry)
            wrapped_symbol_index += 1

        self._strtab_section.data += b''.join(strtab)
        self._symtab_section.data += b''.join(symtab)

        # Make all calls call the wrapper instead.
        for offset, rela_section, rela in self._relas:
            symbol_offset = 24 * rela.symbol_index
            symbol_name = self._symbol_name_by_offset[symbol_offset]
            rela.symbol_index = symbol_name_to_wrapped_index[symbol_name]
            rela_section.data[offset:offset + 24] = rela.data

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

    def _load_symbols(self, symbol_names):
        symtab_data = self._symtab_section.data

        for offset in range(0, len(symtab_data), 24):
            binding = (symtab_data[offset + 4] >> 4)

            if binding != STB_GLOBAL:
                continue

            name_offset, size = self.ST_SYMTAB.unpack_from(symtab_data, offset)

            if size == 0:
                continue

            name = self._find_string_in_strtab(name_offset)

            if name not in symbol_names:
                continue

            self._symbol_names.append(name)
            self._symbol_name_by_offset[offset] = name

    def _load_rela(self):
        for rela_section in self._rela_sections:
            for offset in range(0, len(rela_section.data), 24):
                rela = Elf64LsbRela(rela_section.data[offset:offset + 24])
                symbol_offset = 24 * rela.symbol_index

                if symbol_offset not in self._symbol_name_by_offset:
                    continue

                self._relas.append((offset, rela_section, rela))

    def _find_section_by_name(self, name):
        for section in self._sections:
            if self._find_string_in_shstrtab(section.header.sh_name) == name:
                return section

        raise Error(f'Section called {name} not found.')

    def _find_sections_by_type(self, sh_type):
        return [
            section
            for section in self._sections
            if section.header.sh_type == sh_type
        ]

    def _find_string_in_strtab(self, offset):
        if offset not in self._strtab:
            end = self._strtab_section.data.find(b'\x00', offset)
            name = self._strtab_section.data[offset:end]
            self._strtab[offset] = name.decode('utf-8')

        return self._strtab[offset]

    def _find_string_in_shstrtab(self, offset):
        if offset not in self._shstrtab:
            end = self._shstrtab_section.data.find(b'\x00', offset)
            name = self._shstrtab_section.data[offset:end]
            self._shstrtab[offset] = name.decode('utf-8')

        return self._shstrtab[offset]


def wrap_internal_symbols(object_elf, symbol_names):
    """Wrap given symbols when called internally within an object file, as
    GNU ld only wraps calls between objects (for good reasons). This
    allows mocking function calls within a C source file, given that
    the compiler do not inline those.

    Returns the modified elf object, or None if no modifications were
    needed.

    """

    elffile = Elf64File(bytearray(object_elf), set(symbol_names))

    if not elffile.has_symbols_to_wrap():
        return None

    elffile.wrap_symbols()

    return elffile.to_bytes()
