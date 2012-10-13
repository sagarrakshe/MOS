char *error_message(int err_no) {

	switch(err_no){
		case 0: 
			return (char *)"No Error!\n";

		case 1:
			return (char *)"Out of Data!\n";

		case 2:
			return (char *)"Line Limit Exceeded!\n";

		case 3:
			return (char *)"Time Limit Exceeded!\n";

		case 4:
			return (char *)"Opcode Error!\n";

		case 5:
			return (char *)"Operand Error!\n";

		case 6:
			return (char *)"Invalid Page Fault!\n";

	}
	return NULL;
}