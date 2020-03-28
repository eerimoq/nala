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


class Elf64Header:

    def __init__(self,
                 e_type,
                 e_machine,
                 e_version,
                 e_entry,
                 e_phoff,
                 e_shoff,
                 e_flags,
                 e_ehsize,
                 e_phentsize,
                 e_phnum,
                 e_shentsize,
                 e_shnum,
                 e_shstrndx):
        self.e_type = e_type
        self.e_machine = e_machine
        self.e_version = e_version
        self.e_entry = e_entry
        self.e_phoff = e_phoff
        self.e_shoff = e_shoff
        self.e_flags = e_flags
        self.e_ehsize = e_ehsize
        self.e_phentsize = e_phentsize
        self.e_phnum = e_phnum
        self.e_shentsize = e_shentsize
        self.e_shnum = e_shnum
        self.e_shstrndx = e_shstrndx

    @classmethod
    def from_bytes(cls, data, endianness):
        return cls(*struct.unpack(endianness + 'HHIQQQIHHHHHH', data))

    def to_bytes(self, endianness):
        return struct.pack(endianness + 'HHIQQQIHHHHHH',
                           self.e_type,
                           self.e_machine,
                           self.e_version,
                           self.e_entry,
                           self.e_phoff,
                           self.e_shoff,
                           self.e_flags,
                           self.e_ehsize,
                           self.e_phentsize,
                           self.e_phnum,
                           self.e_shentsize,
                           self.e_shnum,
                           self.e_shstrndx)


class Elf64SectionHeader:

    def __init__(self,
                 sh_name,
                 sh_type,
                 sh_flags,
                 sh_addr,
                 sh_offset,
                 sh_size,
                 sh_link,
                 sh_info,
                 sh_addralign,
                 sh_entsize):
        self.sh_name = sh_name
        self.sh_type = sh_type
        self.sh_flags = sh_flags
        self.sh_addr = sh_addr
        self.sh_offset = sh_offset
        self.sh_size = sh_size
        self.sh_link = sh_link
        self.sh_info = sh_info
        self.sh_addralign = sh_addralign
        self.sh_entsize = sh_entsize

    @classmethod
    def from_bytes(cls, data, endianness):
        return cls(*struct.unpack(endianness + 'IIQQQQIIQQ', data))

    def to_bytes(self, endianness):
        return struct.pack(endianness + 'IIQQQQIIQQ',
                           self.sh_name,
                           self.sh_type,
                           self.sh_flags,
                           self.sh_addr,
                           self.sh_offset,
                           self.sh_size,
                           self.sh_link,
                           self.sh_info,
                           self.sh_addralign,
                           self.sh_entsize)


class Elf64Symbol:

    def __init__(self,
                 st_name,
                 st_info,
                 st_other,
                 st_shndx,
                 st_value,
                 st_size):
        self.st_name = st_name
        self.st_info = st_info
        self.st_other = st_other
        self.st_shndx = st_shndx
        self.st_value = st_value
        self.st_size = st_size

    @classmethod
    def from_bytes(cls, data, endianness):
        return cls(*struct.unpack(endianness + 'IBBHQQ', data))

    def to_bytes(self, endianness):
        return struct.pack(endianness + 'IBBHQQ',
                           self.st_name,
                           self.st_info,
                           self.st_other,
                           self.st_shndx,
                           self.st_value,
                           self.st_size)


class Elf64Rela:

    def __init__(self,
                 r_offset,
                 r_info,
                 r_addend):
        self.r_offset = r_offset
        self.r_info = r_info
        self.r_addend = r_addend

    @classmethod
    def from_bytes(cls, data, endianness):
        return cls(*struct.unpack(endianness + 'QQQ', data))

    def to_bytes(self, endianness):
        return struct.pack(endianness + 'QQQ',
                           self.r_offset,
                           self.r_info,
                           self.r_addend)


class Section:

    def __init__(self, header, elf):
        self.header = header
        offset = header.sh_offset
        self.data = bytearray(elf[offset:offset + header.sh_size])

    def __str__(self):
        return (f'Section of type {self.header.sh_type} with {len(self.data)} '
                f'bytes data.')


def load_section_headers(elf, header):
    section_headers = []

    for i in range(header.e_shnum):
        offset = header.e_shoff + i * header.e_shentsize
        header_data = elf[offset:offset + header.e_shentsize]
        section_header = Elf64SectionHeader.from_bytes(header_data, '<')
        section_headers.append(section_header)

    return section_headers


def load_elf_header(elf):
    magic = elf[0:4]

    if magic != b'\x7fELF':
        raise Error(f'Invalid ELF header magic {magic}.')

    file_class = elf[4]
    data_encoding = elf[5]

    if file_class == ELFCLASS64 and data_encoding == ELFDATA2LSB:
        header = Elf64Header.from_bytes(elf[16:64], '<')
    else:
        raise Error('Only little-endian 64-bit files are supported.')

    if header.e_type != ET_REL:
        raise Error('Only relocatable files are supported.')

    return header


class Elf64File:

    def __init__(self, elf):
        self._e_ident = elf[0:16]
        self._header = load_elf_header(elf)
        section_headers = load_section_headers(elf, self._header)
        self._sections = [
            Section(section_header, elf) for section_header in section_headers
        ]
        self._symtab_section = self._find_section(SHT_SYMTAB)
        self._strtab_section = self._find_section(SHT_STRTAB)
        self._rela_text_section = self._find_section(SHT_RELA)
        self._strtab = {}

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
            section_headers.append(section.header.to_bytes('<'))
            section_datas.append(section.data)

        self._header.e_shoff = offset

        return b''.join([self._e_ident, self._header.to_bytes('<')]
                        + section_datas
                        + section_headers)

    def wrap_symbol(self, symbol_name):
        wrapped_symbol_name = f'__wrap_{symbol_name}'

        # Add wrapped string to .strtab.
        name_offset = len(self._strtab_section.data)
        self._strtab_section.data += wrapped_symbol_name.encode('ascii') + b'\x00'

        # Create an undefiend symbol in .symtab.
        symbol = Elf64Symbol(name_offset, STB_GLOBAL << 4, 0, SHN_UNDEF, 0, 0)
        wrapped_symbol_index = len(self._symtab_section.data) // 24
        self._symtab_section.data += symbol.to_bytes('<')

        # Make all calls call the wrapper instead.
        relas = []

        for i in range(len(self._rela_text_section.data) // 24):
            offset = i * 24
            relas.append(
                Elf64Rela.from_bytes(
                    self._rela_text_section.data[offset:offset + 24], '<'))

        for i, rela in enumerate(relas):
            offset = 24 * (rela.r_info >> 32)
            symbol_data = self._symtab_section.data[offset:offset + 24]
            symbol = Elf64Symbol.from_bytes(symbol_data, '<')
            name = self._find_string_in_strtab(symbol.st_name)

            if name != symbol_name:
                continue

            rela.r_info &= 0xffffffff
            rela.r_info |= (wrapped_symbol_index << 32)
            self._rela_text_section.data[24 * i:24 * i + 24] = rela.to_bytes('<')

    def _find_section(self, sh_type):
        for section in self._sections:
            if section.header.sh_type == sh_type:
                return section

        raise Error(f'Section of type {sh_type} not found.')


def wrap_internal_symbols(object_elf, symbol_names):
    """Wrap given symbols when called internally within an object file, as
    GNU ld only wraps calls between objects (for good reasons). This
    allows mocking function calls within a C source file, given that
    the compiler do not inlide those.

    """

    elffile = Elf64File(object_elf)

    for symbol_name in symbol_names:
        elffile.wrap_symbol(symbol_name)

    return elffile.to_bytes()
