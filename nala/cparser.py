import re
import textparser
from textparser import Sequence
from textparser import choice
from textparser import ZeroOrMore
from textparser import OneOrMore
from textparser import tokenize_init
from textparser import Token
from textparser import Optional
from textparser import DelimitedList
from textparser import Tag
from textparser import replace_blocks
from textparser import AnyUntil


class ParamParser(textparser.Parser):

    def tokenize(self, string):
        keywords = set([
        ])

        skip = set([
        ])

        names = {
            'LPAREN':   '(',
            'RPAREN':   ')',
            'LBRACKET': '[',
            'RBRACKET': ']',
            'COMMA':    ',',
            'STAR':     '*',
            'ELLIPSIS': '...'
        }

        token_specs = [
            ('SKIP',       r'[ \r\n\t]+'),
            ('WORD',       r'[A-Za-z0-9_]+'),
            ('COMMA',      r','),
            ('LPAREN',     r'\('),
            ('RPAREN',     r'\)'),
            ('LBRACKET',   r'\['),
            ('RBRACKET',   r'\]'),
            ('STAR',       r'\*'),
            ('ELLIPSIS',   r'\.\.\.'),
            ('ANY',        r'.')
        ]

        tokens, token_regex = tokenize_init(token_specs)

        for mo in re.finditer(token_regex, string, flags=re.MULTILINE):
            kind = mo.lastgroup

            if kind == 'SKIP':
                pass
            else:
                value = mo.group(kind)

                if value in keywords:
                    kind = value

                if value in skip:
                    continue

                if kind in names:
                    kind = names[kind]

                tokens.append(Token(kind, value, mo.start()))

        return tokens

    def grammar(self):
        return AnyUntil('__EOF__')


class Param:

    parser = ParamParser()

    def __init__(self, source):
        tokens = self.parser.parse(source)
        self.name = None
        self.type = None

    def is_char_pointer(self):
        pass

    def is_pointer(self):
        pass


class Function:

    def __init__(self, name, return_type_source, params_source):
        self.name = name
        self.return_type = self.parse_return_type(return_type_source)
        self.params = self.parse_params(params_source)

    def parse_return_type(self, source):
        return source

    def parse_params(self, source):
        params = []

        for param in source.split(','):
            params.append(Param(param))

        return params


class Text:

    def __init__(self, string, begin, end):
        self.string = string
        self.begin = begin
        self.end = end

    @property
    def text(self):
        return self.string[self.begin:self.end]

    def __repr__(self):
        return self.text


class Struct(Text):
    pass


class Typedef(Text):
    pass


def is_new_file(flags):
    return '1' in flags


class IncludeFinder:

    re_linemarker = re.compile(r'^# (\d+) "(.*)"(.*)$')

    def __init__(self, source_lines, expanded_source):
        self.source_lines = source_lines
        self.expanded_source = expanded_source
        self.includes = []
        self.source_file_path = None
        self.source_line = 0
        self.offset = 0

    def process_linemarker(self, text, offset):
        line, path, flags = self.parse_linemarker(text)

        if self.source_file_path is None:
            self.source_file_path = path
        elif self.source_line > 0:
            if is_new_file(flags):
                string = self.expanded_source[self.offset:offset]
                self.source_line += string.count('\n')
                include = self.source_lines[self.source_line - 2].strip()
                self.includes.append(include)
                self.source_line = 0
        elif path == self.source_file_path:
            self.source_line = int(line)
            self.offset = offset

    def parse_linemarker(self, linemarker):
        mo = self.re_linemarker.match(linemarker)

        if not mo:
            raise Exception(f"Invalid linemarker '{linemarker}'")

        return mo.groups()


class Parsed:

    def __init__(self):
        self.functions = {}
        self.structs = {}
        self.typedefs = {}
        self.includes = []


class FileParser(textparser.Parser):

    def tokenize(self, string):
        keywords = set([
            'typedef',
            'struct',
            'union',
            'enum',
            '__extension__',
            '__attribute__',
            '__asm__'
        ])

        skip = set([
            'extern',
            'static',
            '__extension__'
        ])

        names = {
            'LPAREN': '(',
            'RPAREN': ')',
            'LBRACE': '{',
            'RBRACE': '}',
            'SCOLON': ';'
        }

        token_specs = [
            ('SKIP',       r'[ \r\n\t]+'),
            ('LINEMARKER', r'^#.*$'),
            ('WORD',       r'[A-Za-z0-9_]+'),
            ('STRING',     r'L?"(\\"|\\\\|.)*?"'),
            ('SCOLON',     r';'),
            ('LPAREN',     r'\('),
            ('RPAREN',     r'\)'),
            ('LBRACE',     r'\{'),
            ('RBRACE',     r'\}'),
            ('ANY',        r'.')
        ]

        tokens, token_regex = tokenize_init(token_specs)

        for mo in re.finditer(token_regex, string, flags=re.MULTILINE):
            kind = mo.lastgroup

            if kind in ['SKIP', 'STRING', 'ANY']:
                pass
            else:
                value = mo.group(kind)

                if value in keywords:
                    kind = value

                if value in skip:
                    continue

                if kind in names:
                    kind = names[kind]

                tokens.append(Token(kind, value, mo.start()))

        return tokens

    def grammar(self):
        line_marker = 'LINEMARKER'
        block = Sequence('{', '}')
        group = Sequence('(', ')')
        attributes = OneOrMore(Sequence(choice('__attribute__', '__asm__'), group))
        struct_union_enum = choice('struct', 'union', 'enum')
        typedef = Sequence(
            'typedef',
            choice(Sequence(OneOrMore('WORD'),
                            Optional(attributes), ';'),
                   Sequence(struct_union_enum, block, 'WORD',
                            Optional(attributes), ';'),
                   Sequence(struct_union_enum, 'WORD', Optional(block), 'WORD',
                            Optional(attributes), ';'),
                   Sequence(OneOrMore('WORD'), group, group,
                            Optional(attributes), ';'),
                   Sequence(OneOrMore('WORD'), group,
                            Optional(attributes), ';')))
        struct = Sequence('struct', 'WORD', Optional(block),
                          Optional(attributes), ';')
        union = Sequence('union', 'WORD', Optional(block), ';')
        enum = Sequence('enum', Optional('WORD'), Optional(block), ';')
        function = Sequence(Optional(attributes),
                            OneOrMore(choice('struct',
                                             'union',
                                             'enum',
                                             'WORD',
                                             'LINEMARKER')),
                            group,
                            Optional(attributes),
                            choice(';', block))
        anything = Sequence(AnyUntil(choice(';', '__EOF__')), ';')

        return ZeroOrMore(choice(Tag('linemarker', line_marker),
                                 Tag('typedef', typedef),
                                 Tag('struct', struct),
                                 Tag('union', union),
                                 Tag('enum', enum),
                                 Tag('function', function),
                                 Tag('anything', anything)))


def parse(source, expanded_source, mocked_functions):
    """Parse given pre-precessed C string and extract information needed
    to generate mocks for given functions. That is, mocked function
    declarations and their struct pointer parameter types.

    """

    cleaned_expanded_source = replace_blocks(expanded_source)
    cleaned_expanded_source = replace_blocks(cleaned_expanded_source, '(', ')')
    parsed = Parsed()
    include_finder = IncludeFinder(source.splitlines(), expanded_source)

    for kind, tokens in FileParser().parse(cleaned_expanded_source,
                                           token_tree=True):
        if kind == 'linemarker':
            include_finder.process_linemarker(tokens.value, tokens.offset)
        elif kind == 'function':
            if not isinstance(tokens[-1], Token):
                continue

            name = tokens[1][-1].value

            if name not in mocked_functions:
                continue

            begin = tokens[1][0].offset
            end = tokens[1][-1].offset
            return_type_source = expanded_source[begin:end]
            begin = tokens[2][0].offset + 1
            end = tokens[2][1].offset
            params_source = expanded_source[begin:end]
            parsed.functions[name] = Function(name,
                                              return_type_source,
                                              params_source)
        elif kind == 'struct':
            begin = tokens[0].offset
            end = tokens[-1].offset + 1
            name = tokens[1].value
            parsed.structs[name] = Struct(expanded_source, begin, end)
        elif kind == 'typedef':
            begin = tokens[0].offset
            tokens = tokens[1]

            if tokens[-2]:
                end = tokens[-2][0][0][0].offset - 1
            else:
                end = tokens[-1].offset

            name = None
            parsed.typedefs[name] = Typedef(expanded_source, begin, end)

    parsed.includes = include_finder.includes

    for name, function in parsed.functions.items():
        print(function)

    return parsed


class FastParseResult:

    def __init__(self):
        self.typedefs = []
        self.func_names = []
        self.func_signatures = []
        self.func_source_contexts = []


def fast_parse(expanded_source, mocked_functions):
    cleaned_expanded_source = replace_blocks(expanded_source)
    cleaned_expanded_source = replace_blocks(cleaned_expanded_source, '(', ')')
    result = FastParseResult()
    source_context = []
    re_linemarker = re.compile(r'^# \d+ "(.*)"(.*)$')

    for kind, tokens in FileParser().parse(cleaned_expanded_source,
                                           token_tree=True):
        if kind == 'linemarker':
            path, flags = re_linemarker.match(tokens.value).groups()

            if '1' in flags:
                source_context.append(path)
            elif '2' in flags:
                source_context.pop()

            try:
                source_context[-1] = path
            except IndexError:
                source_context.append(path)
        elif kind == 'function':
            if not isinstance(tokens[-1], Token):
                continue

            name = tokens[1][-1].value

            if name not in mocked_functions:
                continue

            start = tokens[1][0].offset
            end = tokens[2][1].offset
            result.func_names.append(name)
            result.func_signatures.append(expanded_source[start:end + 1] + ';')
            result.func_source_contexts.append(source_context[:])
            mocked_functions.remove(name)

            if not mocked_functions:
                break
        elif kind == 'typedef':
            start = tokens[0].offset
            tokens = tokens[1]

            if tokens[-2]:
                end = tokens[-2][0][0][0].offset - 1
            else:
                end = tokens[-1].offset

            result.typedefs.append(expanded_source[start:end] + ';')

    return result
