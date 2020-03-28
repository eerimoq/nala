import struct
from dataclasses import dataclass


ELFCLASS64 = 2

ELFDATA2LSB = 1

ET_REL = 1

SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4
SHT_REL = 9


class Error(Exception):
    pass


class Symbol:

    def __init__(self):
        pass

@dataclass
class Elf64Header:

    e_type: int
    e_machine: int
    e_version: int
    e_entry: int
    e_phoff: int
    e_shoff: int
    e_flags: int
    e_ehsize: int
    e_phentsize: int
    e_phnum: int
    e_shentsize: int
    e_shnum: int
    e_shstrndx: int

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


@dataclass
class Elf64SectionHeader:

    sh_name: int
    sh_type: int
    sh_flags: int
    sh_addr: int
    sh_offset: int
    sh_size: int
    sh_link: int
    sh_info: int
    sh_addralign: int
    sh_entsize: int

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


@dataclass
class Elf64Symbol:

    st_name: int
    st_info: int
    st_other: int
    st_shndx: int
    st_value: int
    st_size: int

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


class Section:

    def __init__(self, header, elf):
        self.header = header
        self.data = elf[header.sh_offset:header.sh_offset + header.sh_size]

    def to_bytes(self, offset, endianness):
        alignment = self.header.sh_addralign

        if alignment != 0:
            padding_size = (alignment - (offset % alignment))

            if padding_size == alignment:
                padding_size = 0
        else:
            padding_size = 0

        padding = b'\x00' * padding_size
        offset += padding_size
        self.header.sh_offset = offset

        return padding + self.data

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
            offset += len(section.data)
            section_headers.append(section.header.to_bytes('<'))
            section_datas.append(section.data)

        self._header.e_shoff = offset

        return b''.join([self._e_ident, self._header.to_bytes('<')]
                        + section_datas
                        + section_headers)

    def add_undefined_symbol(self, symbol_name):
        pass

    def _find_section(self, sh_type):
        for section in self._sections:
            if section.header.sh_type == sh_type:
                return section

        raise Error(f'Section of type {sh_type} not found.')


def wrap_symbols(object_elf, symbol_names):
    """Wrap given symbols when called internally within an object file, as
    GNU ld only wraps calls between objects. This allows mocking
    function calls within a C source file. However, this is only done
    for global symbols, as local symbols likely will introduce name
    conflicts when in the same global namespace.

    """

    elffile = Elf64File(object_elf)

    for symbol_name in symbol_names:
        elffile.add_undefined_symbol(symbol_name)

    return elffile.to_bytes()
