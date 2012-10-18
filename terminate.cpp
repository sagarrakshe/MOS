char *error_message(int err_no) {

	switch(err_no){
		case 0: 
			return (char *)"No Error!";

		case 1:
			return (char *)"Out of Data!";

		case 2:
			return (char *)"Line Limit Exceeded!";

		case 3:
			return (char *)"Time Limit Exceeded!";

		case 4:
			return (char *)"Opcode Error!";

		case 5:
			return (char *)"Operand Error!";

		case 6:
			return (char *)"Invalid Page Fault!";
	}
	return NULL;
}