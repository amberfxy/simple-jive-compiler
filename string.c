typedef struct String
{
	char *data;
	long count;
} String;

#define PRINT_STRING(s) (int)(s).count, (s).data

#define str_lit(s) (String){(char *)(s), sizeof(s) - 1}

String str_from_cstr(const char *cstr)
{
	String result = {(char *)cstr, strlen(cstr)};
	return result;
}