int main()

{

	char *s="*    hello world    *";

	char *t="*********************";

        int i;

	unsigned short *vga=0xB8000;

	for(i=0;i<25*80;i++)

		*vga++=0;

	vga=0xB8000+1500;

	for(i=0;t[i];i++)

		*vga++=0x0700+'*';

	vga=0xb8000+1500+160;

	for(i=0;s[i];i++)

		*vga++=0x0700+s[i];

	vga=0xb8000+1500+320;
	for(i=0;t[i];i++)

		*vga++=0x0700+'*';

       	return 0;

}
