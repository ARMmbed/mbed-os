/*
 * testblecdsa.cpp
 *
 * Created: 25.10.2017 12:20:44
 *  Author: Guenter.Prossliner
 */


#include <stdio.h>

extern "C" {
    int bl_ecdsa_validate_hash(uint32_t *hash, uint32_t *pk, uint32_t *sig);
    int ecc_ecdsa_validate(const uint32_t *x, const uint32_t *y, const uint32_t *e, const uint32_t *r, const uint32_t *s);

};


int testblecdsa()
{



    // NOK NEW!
    uint32_t pubx[] = {0xA14DDD6C, 0x43F9223C, 0x69177D35, 0xA900FC19, 0xE6F81304, 0x4A6456DE, 0x23137AD5, 0x5EF8A2D3};
    uint32_t puby[] = {0x1B44945A, 0x2EFEED18, 0xE35EC97F, 0x2EF16341, 0x6203D844, 0x184D0AFA, 0x6BD934AD, 0xBFE8A50F};
    uint32_t hash[] = {0x3BEEDE46, 0x21072CA7, 0xF9F435E0, 0x60E68A1E, 0x34247AEC, 0x99561255, 0x06F7A152, 0xD9532266};
    uint32_t sigr[] = {0xC87316ED, 0xE653627E, 0x02218D66, 0x72095BF0, 0xD8893CB4, 0x6FEE1BE7, 0x66D35200, 0x8849B4AA};
    uint32_t sigs[] = {0xDFCD1765, 0x1E54A623, 0xACB3D272, 0x43509EAA, 0x6FD6C49F, 0x251065E3, 0x19A1E05D, 0x07F07476};
    uint32_t *sig = sigr;
    uint32_t *pk = pubx;


    /*
    // OK NEW!
    uint32_t keytp[] = {0xB886082B, 0xB3645FFE, 0x89BFFA44, 0x69B92EA8, 0x59887799, 0x3648971F, 0xBCEE5E6A, 0x2ED509F5};
    uint32_t pubx[] = {0xA14DDD6C, 0x43F9223C, 0x69177D35, 0xA900FC19, 0xE6F81304, 0x4A6456DE, 0x23137AD5, 0x5EF8A2D3};
    uint32_t puby[] = {0x1B44945A, 0x2EFEED18, 0xE35EC97F, 0x2EF16341, 0x6203D844, 0x184D0AFA, 0x6BD934AD, 0xBFE8A50F};
    uint32_t hash[] = {0x3BEEDE46, 0x21072CA7, 0xF9F435E0, 0x60E68A1E, 0x34247AEC, 0x99561255, 0x06F7A152, 0xD9532266};
    uint32_t sigr[] = {0x2BF03E95, 0x65019157, 0x895B9E74, 0x6121E1B6, 0x57F4F51C, 0xA1E237C3, 0x95AE41F4, 0xBF51486F};
    uint32_t sigs[] = {0x62A80A81, 0x23BA1517, 0x4E5A8C09, 0x3AAFEB71, 0x77B34332, 0xBF567AF0, 0xB363CFC2, 0x113B05B2};


    uint32_t * sig = sigr;
    uint32_t * pk = pubx;
    */


    /*
    // TEST NEW OK, deterministic ECDSA...
    uint32_t pubx[] = { 0xA14DDD6C, 0x43F9223C, 0x69177D35, 0xA900FC19, 0xE6F81304, 0x4A6456DE, 0x23137AD5, 0x5EF8A2D3 };
    uint32_t puby[] = { 0x1B44945A, 0x2EFEED18, 0xE35EC97F, 0x2EF16341, 0x6203D844, 0x184D0AFA, 0x6BD934AD, 0xBFE8A50F };
    uint32_t hash[] = { 0xED8875FE, 0x5C55E9A5, 0x165A0B48, 0xCDDC58B5, 0x35C5E141, 0x5DE64684, 0x0C44FADC, 0x29884727 };
    uint32_t sigr[] = { 0x5E256785, 0xDB41E7BD, 0x68345931, 0x0C6C90AF, 0x16FE635F, 0x83403FAB, 0x97F39A9C, 0x557C03D8 };
    uint32_t sigs[] = { 0x69A246BB, 0x8680CFA7, 0xE40FF608, 0xCCA01FCB, 0x210DE92E, 0xF457438D, 0xCF69056C, 0x1D0F8576 };
    uint32_t * sig = sigr;
    uint32_t * pk = pubx;
        */
    /*
        // OK OLD!


    //HASH: 0x5ED5F765, 0xE939258B, 0x3EE0FD95, 0x6CB7C7BB, 0x5F53D95F, 0xBDF33359, 0xF6F6A347, 0xFA920F60
    //SIGR: 0x8C626868, 0x8FE64C6F, 0x0795963D, 0xBFB8BF73, 0xDEE41632, 0xD9BE4215, 0xE5C0B305, 0x4A779E57
    //SIGS: 0xE517D962, 0xAC8FAADB, 0x765A9D3C, 0x00752DD1, 0x181BAC38, 0xC4FB7020, 0x4DDC4BF6, 0x3824D396


    uint32_t hash[] = {0x5ED5F765, 0xE939258B, 0x3EE0FD95, 0x6CB7C7BB, 0x5F53D95F, 0xBDF33359, 0xF6F6A347, 0xFA920F60};
    uint32_t sig[] = {0x8C626868, 0x8FE64C6F, 0x0795963D, 0xBFB8BF73, 0xDEE41632, 0xD9BE4215, 0xE5C0B305, 0x4A779E57,
    0xE517D962, 0xAC8FAADB, 0x765A9D3C, 0x00752DD1, 0x181BAC38, 0xC4FB7020, 0x4DDC4BF6, 0x3824D396};

    uint32_t pk[] = {0xA14DDD6C, 0x43F9223C, 0x69177D35, 0xA900FC19, 0xE6F81304, 0x4A6456DE, 0x23137AD5, 0x5EF8A2D3,
    0x1B44945A, 0x2EFEED18, 0xE35EC97F, 0x2EF16341, 0x6203D844, 0x184D0AFA, 0x6BD934AD, 0xBFE8A50F};
    */



    if (bl_ecdsa_validate_hash(hash, pk, sig) == 0) {
        printf("OK!");
    }

    if (ecc_ecdsa_validate(pubx, puby, hash, sigr, sigs) == 0) {
        printf("OK!");
    }

    sig[0] = 0;
    if (!bl_ecdsa_validate_hash(hash, pk, sig) == 0) {
        printf("OK!");
    }

    if (ecc_ecdsa_validate(pubx, puby, hash, sigr, sigs) == 0) {
        printf("OK!");
    }
}