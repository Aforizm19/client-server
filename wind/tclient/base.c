#include <string.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

void base64e(const char* str, int len)
{
	BIO *bmem, *b64;
 	BUF_MEM *bptr;
	char* buff;

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, str, len);
	BIO_flush(b64);

	BIO_get_mem_ptr(b64, &bptr);

	buff = (char *)malloc(bptr->length);
	memcpy(buff, bptr->data, bptr->length-1);
	buff[bptr->length-1] = 0;

	printf("%s\n", buff);

	BIO_free_all(b64);
	free(buff);
}

void base64d(const char* str, int len)
{
	BIO *bmem, *b64;
	char* in, *out;

	in = (char *)malloc(len + 2);
	out = (char *)malloc(len);
	sprintf(in, "%s\n", str);
	memset(out, 0, len);

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new_mem_buf((unsigned char*)in, strlen(in));
	bmem = BIO_push(b64, bmem);

	BIO_read(bmem, out, len);

	printf("%s\n", out);

	BIO_free_all(bmem);
	free(in);
	free(out);
}

int main(int argc, char* argv[])
{
	int enc;

	if (argc < 3)
	{
		printf("укажите направление (d | e) и данные\n");
		return 1;
	}


	enc = argv[1][0] == 'e' ? 0 : (argv[1][0] == 'd' ? 1 : -1);

	switch (enc)
	{
		case 0:
			base64e(argv[2], strlen(argv[2]));
			break;
		case 1:
			base64d(argv[2], strlen(argv[2]));
			break;
		default:
			printf("неверный ключ\n");
			return 2;
	}

	return 0;
}
