int test_conf(void) {
	unsigned long regA, regB, regC, regD;

	rtc_read(RTC_REGA,&regA);
	rtc_read(RTC_REGB,&regB);
	rtc_read(RTC_REGC,&regC);
	rtc_read(RTC_REGD,&regD);

	printf("REGISTER A: 0x%X\nREGISTER B: 0x%X\nREGISTER C: 0x%X\nREGISTER D: 0x%X\n", regA, regB, regC, regD);
	return 0;
}

int test_date(void) {
	/* To be completed */
}

int test_int(/* to be defined in class */) {
	/* To be completed */
}
