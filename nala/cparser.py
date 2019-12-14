import re
import textparser
from textparser import Sequence
from textparser import choice
from textparser import ZeroOrMore
from textparser import OneOrMore
from textparser import tokenize_init
from textparser import Token
from textparser import Optional
from textparser import Forward
from textparser import Tag
from textparser import replace_blocks


class Parsed:

    def __init__(self):
        self.functions = []


class Parser(textparser.Parser):

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
        typedef = Sequence(
            'typedef',
            choice(Sequence(OneOrMore('WORD'), Optional(attributes), ';'),
                   Sequence('struct', block, 'WORD', Optional(attributes), ';'),
                   Sequence('struct', 'WORD', Optional(block),
                            'WORD', Optional(attributes), ';'),
                   Sequence('enum', block, 'WORD', Optional(attributes), ';'),
                   Sequence('enum', 'WORD', 'WORD', Optional(attributes), ';'),
                   Sequence('union', block, 'WORD', Optional(attributes), ';'),
                   Sequence('union', 'WORD', 'WORD', Optional(attributes), ';'),
                   Sequence('WORD', choice('WORD', group),
                            group, Optional(attributes), ';')))
        struct = Sequence('struct', 'WORD', Optional(block), ';')
        union = Sequence('union', 'WORD', Optional(block), ';')
        enum = Sequence('enum', Optional('WORD'), Optional(block), ';')
        variable = Sequence(Optional('struct'),
                            OneOrMore(choice('WORD', 'LINEMARKER')),
                            Optional(block), ';')
        function = Sequence(Optional(attributes),
                            OneOrMore(choice('struct',
                                             'union',
                                             'enum',
                                             'WORD',
                                             'LINEMARKER')),
                            group,
                            Optional(attributes),
                            choice(';', block))

        return ZeroOrMore(choice(line_marker,
                                 typedef,
                                 struct,
                                 union,
                                 enum,
                                 variable,
                                 Tag('function', function)))


def parse(string):
    """Parse given pre-precessed C string and return the parse tree. To
    speed up the parsing () and {} blocks are empty.

    """

    string = replace_blocks(string)
    string = replace_blocks(string, '(', ')')

    return Parser().parse(string)
