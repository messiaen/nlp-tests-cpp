#ifndef _BLINGFIRE_H_
#define _BLINGFIRE_H_

namespace BlingFire {
// SENTENCE PIECE DELIMITER
#define __FASpDelimiter__ 0x2581

// DEFAULT HYPHEN
#define __FADefaultHyphen__ 0x2012

// WHITESPACE [\x0004-\x0020\x007F-\x009F\x00A0\x2000-\x200F\x202F\x205F\x2060\x2420\x2424\x3000\xFEFF]
#define __FAIsWhiteSpace__(C) ( \
        (C <=  0x20 || C == 0xa0   || (C >= 0x2000 && C <= 0x200f) || \
        C == 0x202f || C == 0x205f || C == 0x2060 || C == 0x2420 || \
        C == 0x2424 || C == 0x3000 || C == 0xfeff) \
    )

// Forward declare
struct FAModelData;

//
// returns the current version of the algo
//
extern "C"
const int GetBlingFireTokVersion();

//
// The same as TextToSentences, but it allows to use a custom model and returns offsets
// 
// pStartOffsets is an array of integers (first character of each sentence) with upto MaxOutUtf8StrByteCount elements
// pEndOffsets is an array of integers (last character of each sentence) with upto MaxOutUtf8StrByteCount elements
//
// The hModel parameter allows to use a custom model loaded with LoadModel API, if NULL then
//  the built in is used.
//

extern "C"
const int TextToSentencesWithOffsetsWithModel(const char * pInUtf8Str, int InUtf8StrByteCount,
    char * pOutUtf8Str, int * pStartOffsets, int * pEndOffsets, const int MaxOutUtf8StrByteCount,
    void * hModel);


//
// The same as TextToSentences, but this one also returns original offsets from the input buffer for each sentence.
//
// pStartOffsets is an array of integers (first character of each sentence) with upto MaxOutUtf8StrByteCount elements
// pEndOffsets is an array of integers (last character of each sentence) with upto MaxOutUtf8StrByteCount elements
//
extern "C"
const int TextToSentencesWithOffsets(const char * pInUtf8Str, int InUtf8StrByteCount,
    char * pOutUtf8Str, int * pStartOffsets, int * pEndOffsets, const int MaxOutUtf8StrByteCount);

//
// The same as TextToSentences, but allows to load a custom model
// 
// The hModel parameter allows to use a custom model loaded with LoadModel API, if NULL then
//  the built in is used.
//
extern "C"
const int TextToSentencesWithModel(const char * pInUtf8Str, int InUtf8StrByteCount, char * pOutUtf8Str, const int MaxOutUtf8StrByteCount, void * hModel);

//
// Splits plain-text in UTF-8 encoding into sentences.
//
// Input:  UTF-8 string of one paragraph or document
// Output: Size in bytes of the output string and UTF-8 string of '\n' delimited sentences, if the return size <= MaxOutUtf8StrByteCount
//
//  Notes:  
//
//  1. The return value is -1 in case of an error (make sure your input is a valid UTF-8, BOM is not required)
//  2. We do not use "\r\n" to delimit sentences, it is always '\n'
//  3. It is not necessary to remove '\n' from the input paragraph, sentence breaking algorithm will take care of them
//  4. The output sentences will not contain '\n' in them
//
extern "C"
const int TextToSentences(const char * pInUtf8Str, int InUtf8StrByteCount, char * pOutUtf8Str, const int MaxOutUtf8StrByteCount);


//
// Same as TextToWords, but also returns original offsets from the input buffer for each word and allows to use a 
//  custom model
//
// pStartOffsets is an array of integers (first character of each word) with upto MaxOutUtf8StrByteCount elements
// pEndOffsets is an array of integers (last character of each word) with upto MaxOutUtf8StrByteCount elements
//
// The hModel parameter allows to use a custom model loaded with LoadModel API, if NULL then
//  the built in is used.
//
extern "C"
const int TextToWordsWithOffsetsWithModel(const char * pInUtf8Str, int InUtf8StrByteCount,
    char * pOutUtf8Str, int * pStartOffsets, int * pEndOffsets, const int MaxOutUtf8StrByteCount,
    void * hModel);

//
// Same as TextToWords, but also returns original offsets from the input buffer for each word.
//
// pStartOffsets is an array of integers (first character of each word) with upto MaxOutUtf8StrByteCount elements
// pEndOffsets is an array of integers (last character of each word) with upto MaxOutUtf8StrByteCount elements
//
extern "C"
const int TextToWordsWithOffsets(const char * pInUtf8Str, int InUtf8StrByteCount,
    char * pOutUtf8Str, int * pStartOffsets, int * pEndOffsets, const int MaxOutUtf8StrByteCount);

//
// Same as TextToWords, but allows to load a custom model
// 
// The hModel parameter allows to use a custom model loaded with LoadModel API, if NULL then
//  the built in is used.
//
extern "C"
const int TextToWordsWithModel(const char * pInUtf8Str, int InUtf8StrByteCount,
    char * pOutUtf8Str, const int MaxOutUtf8StrByteCount, void * hModel);

//
// Splits plain-text in UTF-8 encoding into words.
//
// Input:  UTF-8 string of one sentence or paragraph or document
// Output: Size in bytes of the output string and UTF-8 string of ' ' delimited words, if the return size <= MaxOutUtf8StrByteCount
//
//  Notes:
//
//  1. The return value is -1 in case of an error (make sure your input is a valid UTF-8, BOM is not required)
//  2. Words from the word-breaker will not contain spaces
//
extern "C"
const int TextToWords(const char * pInUtf8Str, int InUtf8StrByteCount, char * pOutUtf8Str, const int MaxOutUtf8StrByteCount);

//
// This function is like TextToWords, but it only normalizes consequtive spaces, it is not as flexble
//  as TextToWords as it cannot take a tokenization and normalization rules, but it does space normalization
//  and srinking multiple spaces to one faster.
//
// Input:  UTF-8 string of one sentence or paragraph or document
//         space character to use, default is __FASpDelimiter__ (U+x2581)
// Output: Size in bytes of the output string and UTF-8 string of ' ' delimited words, if the return size <= MaxOutUtf8StrByteCount
//
// Notes:
//  The return value is -1 in case of an error (make sure your input is a valid UTF-8, BOM is not required)
//  
extern "C"
const int NormalizeSpaces(const char * pInUtf8Str, int InUtf8StrByteCount, char * pOutUtf8Str, const int MaxOutUtf8StrByteCount, const int uSpace = __FASpDelimiter__);

//
// This function implements the fasttext-like hashing logics. It assumes the input text is already tokenized and 
//  tokens are merged by a single space.
//
// Example: input :  "This is ok ."
//          output (bigram):  hash(this), hash(is), hash(ok), hash(.), hash(this is), hash(is ok), hash(ok .), hash(. EOS)
//
//  This is different from fasttext hashing:
//     1. no EOS padding by default for unigram.
//     2. do not take vocab as input, all unigrams are hashed too (we assume later caller repalces unigram hashes
//          with ids from a vocab file).
//
extern "C"
const int TextToHashes(const char * pInUtf8Str, int InUtf8StrByteCount, int32_t * pHashArr, const int MaxHashArrLength, int wordNgrams, int bucketSize = 2000000);

//
// WordHyphenationWithModel - returns a hyphenated string for an input word.
// 
// The hModel parameter allows to use a model loaded with LoadModel API, 
//  NULL is not allowed since there is no built in model for this function
//
extern "C"
const int WordHyphenationWithModel(const char * pInUtf8Str, int InUtf8StrByteCount,
    char * pOutUtf8Str, const int MaxOutUtf8StrByteCount, void * hModel, const int uHy = __FADefaultHyphen__);

//
// Helper, sets up pNewModelData object with model data from memory
// Returns 0 in case of an error otherwise initialized pNewModelData object is returned
//
extern "C"
void* SetModelData(FAModelData * pNewModelData, const unsigned char * pImgBytes);

//
// Same as LoadModel except a model is created from a memory pointer
// Returns 0 in case of an error
//
extern "C"
void* SetModel(const unsigned char * pImgBytes, int ModelByteCount);

//
// Loads a model and return a handle.
// Returns 0 in case of an error.
//
extern "C"
void* LoadModel(const char * pszLdbFileName);

//
// Implements a word-piece algorithm. Returns ids of words or sub-words, returns upto MaxIdsArrLength ids,
// the rest of the array is unchanged, so the array can be set to initial length and fill with 0's for padding.
// If pStartOffsets and pEndOffsets are not NULL then fills in the start and end offset for each token.
// Return value is the number of ids copied into the array.
//
// Example:
//  input: Эpple pie.
//  fa_lex output: эpple/WORD э/WORD_ID_1208 pp/WORD_ID_9397 le/WORD_ID_2571 pie/WORD pie/WORD_ID_11345 ./WORD ./WORD_ID_1012
//  TextToIds output: [1208, 9397, 2571, 11345, 1012, ... <unchanged>]
//
extern "C"
const int TextToIdsWithOffsets_wp(
        void* ModelPtr,
        const char * pInUtf8Str,
        int InUtf8StrByteCount,
        int32_t * pIdsArr, 
        int * pStartOffsets, 
        int * pEndOffsets,
        const int MaxIdsArrLength,
        const int UnkId = 0
);

//
// The same as TextToIdsWithOffsets_wp, except does not return offsets
//
extern "C"
const int TextToIds_wp(
        void* ModelPtr,
        const char * pInUtf8Str,
        int InUtf8StrByteCount,
        int32_t * pIdsArr,
        const int MaxIdsArrLength,
        const int UnkId = 0
);

//
// Implements a sentence piece algorithm, returns predictions from FATokenSegmentationTools_1best_t.
// The input is always prepended with ' ' / '▁' since this seems the case in the sentence piece.
// Returns upto MaxIdsArrLength ids, the rest of the array is unchanged, so the array can be set to 
// initial length and fill with 0's for padding. Returns number of ids copied into the array.
//
// Example:
// printf "Sergei Alonichau I saw a girl with a \ttelescope." | spm_encode --model=xlnet/spiece.model 
// ▁Sergei ▁Al oni chau ▁I ▁saw ▁a ▁girl ▁with ▁a ▁telescope .
//
// printf "Sergei Alonichau I saw a girl with a \ttelescope." | spm_encode --model=xlnet/spiece.model --output_format=id
// 14363 651 7201 25263 35 685 24 1615 33 24 16163 9
//
// TextToIds_sp output: 12, [14363 651 7201 25263 35 685 24 1615 33 24 16163 9]
//
extern "C"
const int TextToIdsWithOffsets_sp(
        void* ModelPtr,
        const char * pInUtf8Str,
        int InUtf8StrByteCount,
        int32_t * pIdsArr,
        int * pStartOffsets, 
        int * pEndOffsets,
        const int MaxIdsArrLength,
        const int UnkId = 0
);

//
// The same as TextToIdsWithOffsets_sp, except does not return offsets
//
extern "C"
const int TextToIds_sp(
        void* ModelPtr,
        const char * pInUtf8Str,
        int InUtf8StrByteCount,
        int32_t * pIdsArr,
        const int MaxIdsArrLength,
        const int UnkId = 0
);

//
// Implements a word-piece or sentence piece algorithms which is defined by the loaded model. 
// Returns ids of words or sub-words, returns upto MaxIdsArrLength ids, the rest of the array 
// is unchanged, so the array can be set to initial length and fill with 0's for padding.
// Returns number of ids copied into the array.
//

extern "C"
const int TextToIdsWithOffsets(
        void* ModelPtr,
        const char * pInUtf8Str,
        int InUtf8StrByteCount,
        int32_t * pIdsArr,
        int * pStartOffsets, 
        int * pEndOffsets,
        const int MaxIdsArrLength,
        const int UnkId = 0
);

//
// Implements a word-piece or sentence piece algorithms which is defined by the loaded model. 
// Returns ids of words or sub-words, returns upto MaxIdsArrLength ids, the rest of the array 
// is unchanged, so the array can be set to initial length and fill with 0's for padding.
// Returns number of ids copied into the array.
//

extern "C"
const int TextToIds(
        void* ModelPtr,
        const char * pInUtf8Str,
        int InUtf8StrByteCount,
        int32_t * pIdsArr,
        const int MaxIdsArrLength,
        const int UnkId = 0
);

//
// Frees memory from the model, after this call ModelPtr is no longer valid
//  Double calls to this function with the same argument will case access violation
//
extern "C"
int FreeModel(void* ModelPtr);

extern "C"
int SetNoDummyPrefix(void* ModelPtr, bool fNoDummyPrefix);

//
// Returns text string given a sequence of Ids
//  Note: the model file should contain [i2w] configuration or separate *.i2w model file should be used
// 
// return value is the actual string length
// if the actual string length is more than MaxOutUtf8StrByteCount then pOutUtf8Str content is undefined
// 
extern "C"
int IdsToText (void* ModelPtr, const int32_t * pIdsArr, const int IdsCount, char * pOutUtf8Str, const int MaxOutUtf8StrByteCount, bool SkipSpecialTokens);
}
#endif