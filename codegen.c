void generate_preamble(FILE *out_file)
{
	fprintf(out_file, "global _start\n");
	fprintf(out_file, "\n");
	fprintf(out_file, "_start:\n");
	fprintf(out_file, "    call main\n");
	fprintf(out_file, "    mov rdi, rax\n");
	fprintf(out_file, "    mov rax, 60\n");
	fprintf(out_file, "    syscall\n");
	fprintf(out_file, "\n");
}

// Helper function to generate asm for an expression
bool generate_asm_for_expr(AST_Node *expr, FILE *out_file)
{
	if (expr == NULL)
	{
		printf("ERROR: NULL expression in code generation\n");
		return false;
	}
	
	switch (expr->kind)
	{
	case AST_INTEGER:
		// Load the integer value into rax
		fprintf(out_file, "    mov rax, %ld\n", expr->int_value);
		return true;
	
	default:
		printf("ERROR: Unhandled expression kind %s in code generation\n", ast_kind_as_cstr(expr->kind));
		return false;
	}
}

// Helper function to generate asm for a statement
bool generate_asm_for_stmt(AST_Node *stmt, FILE *out_file)
{
	if (stmt == NULL)
	{
		printf("ERROR: NULL statement in code generation\n");
		return false;
	}
	
	switch (stmt->kind)
	{
	case AST_RETURN:
		if (stmt->ret_expr != NULL)
		{
			// Generate code for the return expression
			bool success = generate_asm_for_expr(stmt->ret_expr, out_file);
			if (!success) return false;
		}
		// Return from the function (rax already contains the return value)
		fprintf(out_file, "    ret\n");
		return true;
	
	default:
		printf("ERROR: Unhandled statement kind %s in code generation\n", ast_kind_as_cstr(stmt->kind));
		return false;
	}
}

bool generate_asm_for_fn(AST_Node *fn_node, FILE *out_file)
{
	// TODO: Implement this
	// TODO: Print the function name as a label
	// TODO: Iterate over the body of the function
	//       TODO: For each statement, generate the asm
	//       NOTE: Only return statements need to be supported for now
	//             and the only expression you need to handle is an integer literal
	// TODO: Return true on successful generation, and false on error
	
	if (fn_node == NULL || fn_node->kind != AST_FN)
	{
		printf("ERROR: Expected function node in generate_asm_for_fn\n");
		return false;
	}
	
	// Print the function name as a label
	fprintf(out_file, "%.*s:\n", PRINT_STRING(fn_node->fn.name));
	
	// Iterate over the body of the function
	for (AST_Node *stmt = fn_node->fn.body.first; stmt != NULL; stmt = stmt->next)
	{
		bool success = generate_asm_for_stmt(stmt, out_file);
		if (!success) return false;
	}
	
	fprintf(out_file, "\n");
	
	return true;
}

bool generate_asm(AST_Node *ast, FILE *out_file)
{
	if (ast == NULL || ast->kind != AST_PROGRAM)
	{
		printf("ERROR: Root AST node was not PROGRAM. Got kind %s\n", ast_kind_as_cstr(ast->kind));
		return false;
	}
	
	generate_preamble(out_file);
	
	for (AST_Node *fn_node = ast->program.first; fn_node != NULL; fn_node = fn_node->next)
	{
		bool success = generate_asm_for_fn(fn_node, out_file);
		if (!success) return false;
	}
	
	return true;
}