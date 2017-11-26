#ifndef DIGITAL_SIGNATURE_GENERATOR_H_
#define DIGITAL_SIGNATURE_GENERATOR_H_

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct {
	string name;
	string value;
} paraPair;

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32_sh
#define uint32_sh unsigned long int
#endif

typedef struct
{
	uint32_sh total[2];
	uint32_sh state[5];
	uint8 buffer[64];
}
sha1_context;

class SignatureGenerator
{
private:

	void sha1_process( sha1_context *ctx, uint8 data[64] );
	void sha1_starts( sha1_context *ctx );
	void sha1_update( sha1_context *ctx, uint8 *input, unsigned long int length );
	void sha1_finish( sha1_context *ctx, uint8 digest[20] );

public:

	SignatureGenerator();

	~SignatureGenerator();
	void getSignature(string openId, string openKey, vector<paraPair> &paraList, string &signature);

};

#endif
