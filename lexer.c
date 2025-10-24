typedef enum Token_Kind
{
	TOKEN_NONE = 0,
	TOKEN_EOF,
	TOKEN_IDENT,
	TOKEN_INTEGER,
	TOKEN_KEYWORD,
	TOKEN_TYPE,
	TOKEN_ARROW, // ->
	
	// Single character tokens (ASCII values)
	// We can use the character itself as the token kind
	// For example: '(', ')', '{', '}', etc.
} Token_Kind;

typedef enum Keyword
{
	KEYWORD_NONE = 0,
	KEYWORD_fn,
	KEYWORD_return,
	// Add more keywords as needed
} Keyword;

const String keyword_names[] = {
	[KEYWORD_NONE]   = str_lit("NONE"),
	[KEYWORD_fn]     = str_lit("fn"),
	[KEYWORD_return] = str_lit("return"),
};

typedef enum Type
{
	TYPE_NONE = 0,
	TYPE_int,
	// Add more types as needed
} Type;

const String type_names[] = {
	[TYPE_NONE] = str_lit("NONE"),
	[TYPE_int]  = str_lit("int"),
};

typedef struct Loc
{
	const char *file_name;
	long line;
	long column;
} Loc;

typedef struct Token
{
	Token_Kind kind;
	Loc loc;
	String text;
	
	union // Additional data depending on kind
	{
		Keyword keyword; // For TOKEN_KEYWORD
		Type type;       // For TOKEN_TYPE
		long int_value;  // For TOKEN_INTEGER
	};
} Token;

typedef struct Token_Array
{
	Token *items;
	long count;
	long capacity;
} Token_Array;

void print_loc(Loc loc)
{
	printf("%s:%ld:%ld", loc.file_name, loc.line, loc.column);
}

void print_token_kind(Token_Kind kind)
{
	switch (kind)
	{
	case TOKEN_NONE:    printf("NONE"); break;
	case TOKEN_EOF:     printf("EOF"); break;
	case TOKEN_IDENT:   printf("IDENT"); break;
	case TOKEN_INTEGER: printf("INTEGER"); break;
	case TOKEN_KEYWORD: printf("KEYWORD"); break;
	case TOKEN_TYPE:    printf("TYPE"); break;
	case TOKEN_ARROW:   printf("ARROW"); break;
	default:
		if (kind < 128 && isprint(kind))
		{
			printf("'%c'", (char)kind);
		}
		else
		{
			printf("UNKNOWN(%d)", kind);
		}
		break;
	}
}

void print_token(Token *tok)
{
	print_loc(tok->loc);
	printf(": ");
	print_token_kind(tok->kind);
	printf(" '%.*s'", PRINT_STRING(tok->text));
}

void print_token_array(Token_Array tokens)
{
	for (long i = 0; i < tokens.count; i++)
	{
		print_token(&tokens.items[i]);
		printf("\n");
	}
}

typedef struct Lexer
{
	const char *file_name;
	char *source;
	long source_len;
	long pos;
	long line;
	long column;
	
	Token_Array tokens;
} Lexer;

void token_array_append(Token_Array *array, Token token)
{
	if (array->count >= array->capacity)
	{
		array->capacity = array->capacity == 0 ? 16 : array->capacity * 2;
		array->items = realloc(array->items, array->capacity * sizeof(Token));
	}
	array->items[array->count++] = token;
}

char peek_char(Lexer *lexer, long offset)
{
	long index = lexer->pos + offset;
	if (index >= lexer->source_len)
	{
		return '\0';
	}
	return lexer->source[index];
}

char advance_char(Lexer *lexer)
{
	char c = peek_char(lexer, 0);
	if (c != '\0')
	{
		lexer->pos++;
		lexer->column++;
		if (c == '\n')
		{
			lexer->line++;
			lexer->column = 1;
		}
	}
	return c;
}

void skip_whitespace(Lexer *lexer)
{
	while (true)
	{
		char c = peek_char(lexer, 0);
		if (isspace(c))
		{
			advance_char(lexer);
		}
		else if (c == '/' && peek_char(lexer, 1) == '/') // Line comment
		{
			while (peek_char(lexer, 0) != '\n' && peek_char(lexer, 0) != '\0')
			{
				advance_char(lexer);
			}
		}
		else
		{
			break;
		}
	}
}

Keyword match_keyword(String text)
{
	for (int i = 1; i < sizeof(keyword_names) / sizeof(keyword_names[0]); i++)
	{
		if (text.count == keyword_names[i].count &&
		    memcmp(text.data, keyword_names[i].data, text.count) == 0)
		{
			return (Keyword)i;
		}
	}
	return KEYWORD_NONE;
}

Type match_type(String text)
{
	for (int i = 1; i < sizeof(type_names) / sizeof(type_names[0]); i++)
	{
		if (text.count == type_names[i].count &&
		    memcmp(text.data, type_names[i].data, text.count) == 0)
		{
			return (Type)i;
		}
	}
	return TYPE_NONE;
}

Token make_token(Lexer *lexer, Token_Kind kind, long start_pos, long start_column)
{
	Token tok = {0};
	tok.kind = kind;
	tok.loc.file_name = lexer->file_name;
	tok.loc.line = lexer->line;
	tok.loc.column = start_column;
	tok.text.data = &lexer->source[start_pos];
	tok.text.count = lexer->pos - start_pos;
	return tok;
}

void lex_source(Lexer *lexer)
{
	while (true)
	{
		skip_whitespace(lexer);
		
		char c = peek_char(lexer, 0);
		if (c == '\0')
		{
			break;
		}
		
		long start_pos = lexer->pos;
		long start_column = lexer->column;
		
		// Single character tokens
		if (c == '(' || c == ')' || c == '{' || c == '}' || c == ',')
		{
			advance_char(lexer);
			Token tok = make_token(lexer, (Token_Kind)c, start_pos, start_column);
			token_array_append(&lexer->tokens, tok);
		}
		// Arrow ->
		else if (c == '-' && peek_char(lexer, 1) == '>')
		{
			advance_char(lexer);
			advance_char(lexer);
			Token tok = make_token(lexer, TOKEN_ARROW, start_pos, start_column);
			token_array_append(&lexer->tokens, tok);
		}
		// Numbers
		else if (isdigit(c))
		{
			while (isdigit(peek_char(lexer, 0)))
			{
				advance_char(lexer);
			}
			Token tok = make_token(lexer, TOKEN_INTEGER, start_pos, start_column);
			
			// Parse the integer value
			tok.int_value = 0;
			for (long i = 0; i < tok.text.count; i++)
			{
				tok.int_value = tok.int_value * 10 + (tok.text.data[i] - '0');
			}
			
			token_array_append(&lexer->tokens, tok);
		}
		// Identifiers and keywords
		else if (isalpha(c) || c == '_')
		{
			while (isalnum(peek_char(lexer, 0)) || peek_char(lexer, 0) == '_')
			{
				advance_char(lexer);
			}
			Token tok = make_token(lexer, TOKEN_IDENT, start_pos, start_column);
			
			// Check if it's a keyword
			Keyword kw = match_keyword(tok.text);
			if (kw != KEYWORD_NONE)
			{
				tok.kind = TOKEN_KEYWORD;
				tok.keyword = kw;
			}
			// Check if it's a type
			else
			{
				Type type = match_type(tok.text);
				if (type != TYPE_NONE)
				{
					tok.kind = TOKEN_TYPE;
					tok.type = type;
				}
			}
			
			token_array_append(&lexer->tokens, tok);
		}
		else
		{
			printf("ERROR: Unexpected character '%c' at ", c);
			print_loc((Loc){lexer->file_name, lexer->line, lexer->column});
			printf("\n");
			advance_char(lexer);
		}
	}
	
	// Add EOF token
	Token eof_tok = {0};
	eof_tok.kind = TOKEN_EOF;
	eof_tok.loc.file_name = lexer->file_name;
	eof_tok.loc.line = lexer->line;
	eof_tok.loc.column = lexer->column;
	eof_tok.text.data = &lexer->source[lexer->pos];
	eof_tok.text.count = 0;
	token_array_append(&lexer->tokens, eof_tok);
}

Token_Array lex_file(const char *file_name)
{
	// Read the file
	FILE *file = fopen(file_name, "r");
	if (!file)
	{
		printf("ERROR: Could not open file %s\n", file_name);
		return (Token_Array){0};
	}
	
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	char *source = malloc(file_size + 1);
	fread(source, 1, file_size, file);
	source[file_size] = '\0';
	
	fclose(file);
	
	// Lex the source
	Lexer lexer = {
		.file_name = file_name,
		.source = source,
		.source_len = file_size,
		.pos = 0,
		.line = 1,
		.column = 1,
	};
	
	lex_source(&lexer);
	
	return lexer.tokens;
}

