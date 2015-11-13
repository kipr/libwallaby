// This code is a heavily modified version of Pongsak Suvanpong's C++ version (psksvp@robotvision2.com, psksvp@gmail.com)
// based on JavaDrone http://code.google.com/p/javadrone/
// and droneController http://dronecontroller.codeplex.com/

#include "uvlc_video_decoder_p.hpp"

#include <vector>
#include <iostream>
#include <cstring>
#include <cstdio>
#include "bits_p.hpp"

using namespace Private;

const static int kPictureFormatCIF = 1;
const static int kPictureFormatVGA = 2;

#define UVLC_BLOCK_WIDTH (8)
#define UVLC_BLOCK_SIZE (64)

#define UVLC_CIF_WIDTH (88)
#define UVLC_CIF_HEIGHT (72)

#define UVLC_VGA_WIDTH (160)
#define UVLC_VGA_HEIGHT (120)

#define UVLC_START_CODE (0x0020)

const static int CONST_TableQuantization = 31;

const static int CONST_BITS = 13;
const static int PASS1_BITS = 1;

namespace Private
{
	struct Rgb
	{
		Rgb(unsigned char r, unsigned char g, unsigned char b)
			: r(r),
			g(g),
			b(b)
		{
		}
	
		unsigned short rgb16()
		{
			return (unsigned short)((r << 11) | (g << 5) | b);
		}
	
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
}

UvlcVideoDecoder::UvlcVideoDecoder()
{
}

UvlcVideoDecoder::~UvlcVideoDecoder()
{
}

void UvlcVideoDecoder::inverseTransform(short dataBlockBuffer[64], short *const block) const
{
	int work[64];
	short data[64];
	
	int z1, z2, z3, z4, z5;
	int tmp0, tmp1, tmp2, tmp3;
	int tmp10, tmp11, tmp12, tmp13;
	
	const static int F1 = CONST_BITS - PASS1_BITS - 1;
	const static int F2 = CONST_BITS - PASS1_BITS + 0;
	const static int F3 = CONST_BITS + PASS1_BITS + 3;
	
	const static int FIX_0_298631336 = 2446;
	const static int FIX_0_390180644 = 3196;
	const static int FIX_0_541196100 = 4433;
	const static int FIX_0_765366865 = 6270;
	const static int FIX_0_899976223 = 7373;
	const static int FIX_1_175875602 = 9633;
	const static int FIX_1_501321110 = 12299;
	const static int FIX_1_847759065 = 15137;
	const static int FIX_1_961570560 = 16069;
	const static int FIX_2_053119869 = 16819;
	const static int FIX_2_562915447 = 20995;
	const static int FIX_3_072711026 = 25172;
	
	size_t pointer = 0;
	for (size_t index = 8; index > 0; --index) {
		if (dataBlockBuffer[pointer + 8] == 0 &&
			dataBlockBuffer[pointer + 16] == 0 &&
			dataBlockBuffer[pointer + 24] == 0 &&
			dataBlockBuffer[pointer + 32] == 0 &&
			dataBlockBuffer[pointer + 40] == 0 &&
			dataBlockBuffer[pointer + 48] == 0 &&
			dataBlockBuffer[pointer + 56] == 0)
		{
			int dcValue = dataBlockBuffer[pointer] << PASS1_BITS;
			
			work[pointer + 0] = dcValue;
			work[pointer + 8] = dcValue;
			work[pointer + 16] = dcValue;
			work[pointer + 24] = dcValue;
			work[pointer + 32] = dcValue;
			work[pointer + 40] = dcValue;
			work[pointer + 48] = dcValue;
			work[pointer + 56] = dcValue;
			
			++pointer;
			continue;
		}
		
		z2 = dataBlockBuffer[pointer + 16];
		z3 = dataBlockBuffer[pointer + 48];
		
		z1 = (z2 + z3) * FIX_0_541196100;
		tmp2 = z1 + z3 * -FIX_1_847759065;
		tmp3 = z1 + z2 * FIX_0_765366865;
		
		z2 = dataBlockBuffer[pointer];
		z3 = dataBlockBuffer[pointer + 32];
		
		tmp0 = (z2 + z3) << CONST_BITS;
		tmp1 = (z2 - z3) << CONST_BITS;
		
		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;
		
		tmp0 = dataBlockBuffer[pointer + 56];
		tmp1 = dataBlockBuffer[pointer + 40];
		tmp2 = dataBlockBuffer[pointer + 24];
		tmp3 = dataBlockBuffer[pointer + 8];
		
		z1 = tmp0 + tmp3;
		z2 = tmp1 + tmp2;
		z3 = tmp0 + tmp2;
		z4 = tmp1 + tmp3;
		z5 = (z3 + z4) * FIX_1_175875602;
		
		tmp0 = tmp0 * FIX_0_298631336;
		tmp1 = tmp1 * FIX_2_053119869;
		tmp2 = tmp2 * FIX_3_072711026;
		tmp3 = tmp3 * FIX_1_501321110;
		z1 = z1 * -FIX_0_899976223;
		z2 = z2 * -FIX_2_562915447;
		z3 = z3 * -FIX_1_961570560;
		z4 = z4 * -FIX_0_390180644;
		
		z3 += z5;
		z4 += z5;
		
		tmp0 += z1 + z3;
		tmp1 += z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;
		
		work[pointer + 0] = ((tmp10 + tmp3 + (1 << F1)) >> F2);
		work[pointer + 56] = ((tmp10 - tmp3 + (1 << F1)) >> F2);
		work[pointer + 8] = ((tmp11 + tmp2 + (1 << F1)) >> F2);
		work[pointer + 48] = ((tmp11 - tmp2 + (1 << F1)) >> F2);
		work[pointer + 16] = ((tmp12 + tmp1 + (1 << F1)) >> F2);
		work[pointer + 40] = ((tmp12 - tmp1 + (1 << F1)) >> F2);
		work[pointer + 24] = ((tmp13 + tmp0 + (1 << F1)) >> F2);
		work[pointer + 32] = ((tmp13 - tmp0 + (1 << F1)) >> F2);
		
		++pointer;
	}
	
	pointer = 0;
	for (size_t index = 0; index < 8; ++index) {
		z2 = work[pointer + 2];
		z3 = work[pointer + 6];
		
		z1 = (z2 + z3) * FIX_0_541196100;
		tmp2 = z1 + z3 * -FIX_1_847759065;
		tmp3 = z1 + z2 * FIX_0_765366865;
		
		tmp0 = (work[pointer + 0] + work[pointer + 4]) << CONST_BITS;
		tmp1 = (work[pointer + 0] - work[pointer + 4]) << CONST_BITS;
		
		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;
		
		tmp0 = work[pointer + 7];
		tmp1 = work[pointer + 5];
		tmp2 = work[pointer + 3];
		tmp3 = work[pointer + 1];
		
		z1 = tmp0 + tmp3;
		z2 = tmp1 + tmp2;
		z3 = tmp0 + tmp2;
		z4 = tmp1 + tmp3;
		
		z5 = (z3 + z4) * FIX_1_175875602;
		
		tmp0 = tmp0 * FIX_0_298631336;
		tmp1 = tmp1 * FIX_2_053119869;
		tmp2 = tmp2 * FIX_3_072711026;
		tmp3 = tmp3 * FIX_1_501321110;
		z1 = z1 * -FIX_0_899976223;
		z2 = z2 * -FIX_2_562915447;
		z3 = z3 * -FIX_1_961570560;
		z4 = z4 * -FIX_0_390180644;
		
		z3 += z5;
		z4 += z5;
		
		tmp0 += z1 + z3;
		tmp1 += z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;
		
		data[pointer + 0] = (short)((tmp10 + tmp3) >> F3);
		data[pointer + 7] = (short)((tmp10 - tmp3) >> F3);
		data[pointer + 1] = (short)((tmp11 + tmp2) >> F3);
		data[pointer + 6] = (short)((tmp11 - tmp2) >> F3);
		data[pointer + 2] = (short)((tmp12 + tmp1) >> F3);
		data[pointer + 5] = (short)((tmp12 - tmp1) >> F3);
		data[pointer + 3] = (short)((tmp13 + tmp0) >> F3);
		data[pointer + 4] = (short)((tmp13 - tmp0) >> F3);
		
		pointer += 8;
	}
	
	::memcpy(block, data, 64 * sizeof(short));
}

int UvlcVideoDecoder::saturate5(int x)
{
	if(x < 0) x = 0;
	x >>= 11;
	return (x > 0x1F) ? 0x1F : x;
}

int UvlcVideoDecoder::saturate6(int x)
{
	if(x < 0) x = 0;
	x >>= 10;
	return x > 0x3F ? 0x3F : x;
}

void UvlcVideoDecoder::composeImageSlice(cv::Mat &image, const MacroBlockVector &imageSlice)
{

	const static int cromaQuadrantOffsets[] = { 0, 4, 32, 36 };
	const int pixelDataQuadrantOffsets[] = {
		0, UVLC_BLOCK_WIDTH,
		image.size().width * UVLC_BLOCK_WIDTH, image.size().width * UVLC_BLOCK_WIDTH + UVLC_BLOCK_WIDTH
	};
	
	size_t imageDataOffset = (SliceIndex - 1) * image.size().width * 16;
	
	MacroBlockVector::const_iterator it = imageSlice.begin();
	for(; it != imageSlice.end(); ++it) {
		const MacroBlock &macroBlock = *it;
		
		for(unsigned vstep = 0; vstep < UVLC_BLOCK_WIDTH / 2; ++vstep) {
			
			const size_t chromaOffset = vstep * UVLC_BLOCK_WIDTH;
			const size_t lumaElementIndex1 = vstep * UVLC_BLOCK_WIDTH * 2;
			const size_t lumaElementIndex2 = lumaElementIndex1 + UVLC_BLOCK_WIDTH;
			
			const size_t dataIndex1 = imageDataOffset + 2 * vstep * image.size().width;
			const size_t dataIndex2 = dataIndex1 + image.size().width;
			
			for(unsigned hstep = 0; hstep < UVLC_BLOCK_WIDTH / 2; ++hstep) {
				for(unsigned quadrant = 0; quadrant < 4; ++quadrant) {
					const size_t chromaIndex = chromaOffset + cromaQuadrantOffsets[quadrant] + hstep;
					int chromaBlueValue = macroBlock.dataBlocks[4][chromaIndex];
					int chromaRedValue = macroBlock.dataBlocks[5][chromaIndex];
					
					int u = chromaBlueValue - 128;
					int ug = 88 * u;
					int ub = 454 * u;
					
					int v = chromaRedValue - 128;
					int vg = 183 * v;
					int vr = 359 * v;
					
					for(unsigned pixel = 0; pixel < 2; ++pixel) {
						const size_t deltaIndex = 2 * hstep + pixel;
						int lumaElementValue1 = macroBlock.dataBlocks[quadrant][lumaElementIndex1 + deltaIndex] << 8;
						int lumaElementValue2 = macroBlock.dataBlocks[quadrant][lumaElementIndex2 + deltaIndex] << 8;
						
						const size_t offset1 = (dataIndex1 + pixelDataQuadrantOffsets[quadrant] + deltaIndex) * 3;
						image.at<unsigned char>(offset1 + 0) = saturate5(lumaElementValue1 + ub) << 3;
						image.at<unsigned char>(offset1 + 1) = saturate6(lumaElementValue1 - ug - vg) << 2;
						image.at<unsigned char>(offset1 + 2) = saturate5(lumaElementValue1 + vr) << 3;
						
						const size_t offset2 = (dataIndex2 + pixelDataQuadrantOffsets[quadrant] + deltaIndex) * 3;
						image.at<unsigned char>(offset2 + 0) = saturate5(lumaElementValue2 + ub) << 3;
						image.at<unsigned char>(offset2 + 1) = saturate6(lumaElementValue2 - ug - vg) << 2;
						image.at<unsigned char>(offset2 + 2) = saturate5(lumaElementValue2 + vr) << 3;
					}
				}
			}
		}
		
		imageDataOffset += 16;
	}
}

void UvlcVideoDecoder::alignStreamData()
{
	int actualLength = StreamFieldBitIndex;
	if(actualLength <= 0) return;
	
	int alignedLength = actualLength & ~7;
	if(alignedLength == actualLength) return;
	alignedLength += 0x08;
	StreamField <<= (alignedLength - actualLength);
	StreamFieldBitIndex = alignedLength;
}


int UvlcVideoDecoder::makeIntFromBytes(const unsigned char *const buffer, int index)
{
	unsigned char b[4];
	b[0] = buffer[index];
	b[1] = buffer[index + 1];
	b[2] = buffer[index + 2];
	b[3] = buffer[index + 3];
	
	int ret;
	::memcpy(&ret, b, sizeof(ret));
	return ret;
}

unsigned int UvlcVideoDecoder::peekStreamData(const unsigned char *const stream, int count)
{
	unsigned int data = 0;
	unsigned int streamField = StreamField;
	int streamFieldBitIndex = StreamFieldBitIndex;
	
	while (count > (32 - streamFieldBitIndex) && StreamIndex < (ImageStreamLength >> 2)) {
		data = (data << (int)(32 - streamFieldBitIndex)) | (streamField >> streamFieldBitIndex);
		
		count -= 32 - streamFieldBitIndex;
		
		streamField = makeIntFromBytes(stream, StreamIndex * 4); //BitConverter.ToUInt32(stream, StreamIndex * 4);
		streamFieldBitIndex = 0;
	}
	
	if(count <= 0) return data;
	return (data << count) | (streamField >> (32 - count));
}

unsigned int UvlcVideoDecoder::readStreamData(int count)
{
	unsigned int data = 0;
	
	while (count > (32 - StreamFieldBitIndex)) {
		data = (data << (int)(32 - StreamFieldBitIndex)) | (StreamField >> StreamFieldBitIndex);
		
		count -= 32 - StreamFieldBitIndex;
		
		StreamField = makeIntFromBytes(ImageStream, StreamIndex * 4);
		StreamFieldBitIndex = 0;
		++StreamIndex;
	}
	
	if(count <= 0) return data;
	
	data = (data << count) | (StreamField >> (32 - count));
	StreamField <<= count;
	StreamFieldBitIndex += count;
	
	return data;
}

void UvlcVideoDecoder::decodeFieldBytes(int &run, int &level, bool &last)
{
	unsigned int streamCode = 0;
	
	int streamLength = 0;
	int temp = 0;
	
	// Use the RLE and Huffman dictionaries to understand this code fragment. You can find 
	// them in the developers guide on page 34.
	// The bits in the data are actually composed of two kinds of fields:
	// - run fields - this field contains information on the number of consecutive zeros.
	// - level fields - this field contains the actual non zero value which can be negative or positive.
	// First we extract the run field info and then the level field info.
	streamCode = peekStreamData(ImageStream, 32);
	
	
	// Suppose we have following bit sequence:
	// 00001111.....
	// 1 - Count the number of leading zeros -> 4
	//     Coarse value lookup is thus 00001
	// 2 - Lookup the additional value, for coarse value 00001 this is 3 addtional bits
	// 3 - Calculate value of run, for coarse value 00001 this is (111) + 8
	
	unsigned char zeroCount = Bits::leadingZeros(streamCode); // - (1)
	streamCode <<= zeroCount + 1; // - (2) -> shift left to get rid of the coarse value
	streamLength += zeroCount + 1; // - position bit pointer to keep track off how many bits to consume later on the stream.
	
	if(zeroCount > 1) {
		temp = (int)(streamCode >> (32 - (zeroCount - 1))); // - (2) -> shift right to determine the addtional bits (number of additional bits is zerocount - 1)
		streamCode <<= zeroCount - 1; // - shift all of the run bits out of the way so the first bit is points to the first bit of the level field.
		streamLength += zeroCount - 1;// - position bit pointer to keep track off how many bits to consume later on the stream.
		run = temp + (1 << (zeroCount - 1)); // - (3) -> calculate run value
	} else run = zeroCount;
	
	// Suppose we have following bit sequence:
	// 000011111.....
	// 1 - Count the number of leading zeros -> 4
	//     Coarse value lookup is thus 00001
	// 2 - Lookup the additional value, for coarse value 00001 this is 4 addtional bits (last bit is sign bit)
	// 3 - Calculate value of run, for coarse value 00001 this is (xxx) + 8, multiply by sign
	
	zeroCount = Bits::leadingZeros(streamCode);
	streamCode <<= zeroCount + 1; // - (1)
	streamLength += zeroCount + 1; // - position bit pointer to keep track off how many bits to consume later on the stream.
	
	if (zeroCount == 1) {
		// If coarse value is 01 according to the Huffman dictionary this means EOB, so there is
		// no run and level and we indicate this by setting last to true;
		run = 0;
		last = true;
	} else {
		if(!zeroCount) {
			zeroCount = 1;
			temp = 1;
		}
		
		streamLength += zeroCount;// - position bit pointer to keep track off how many bits to consume later on the stream.
		streamCode >>= (32 - zeroCount);// - (2) -> shift right to determine the addtional bits (number of additional bits is zerocount)
		int sign = (int)(streamCode & 1); // determine sign, last bit is sign 
		
		if(zeroCount) {
			temp = (int)(streamCode >> 1); // take into account that last bit is sign, so shift it out of the way
			temp += (int)(1 << (zeroCount - 1)); // - (3) -> calculate run value without sign
		}
		
		level = (sign == 1) ? -temp : temp; // - (3) -> calculate run value with sign
		last = false;
	}
	
	
	readStreamData(streamLength);
}

static const short zigZagPositions[] = {
	0,   1,  8, 16,  9,  2,  3, 10,
	17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34,
	27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36,
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46,
	53, 60, 61, 54, 47, 55, 62, 63,
};

// Cfr. Handbook of Data Compression - Page 529
// David Salomon
// Giovanni Motta

static const short quantizerValues[] = {
	3,   5,  7,  9, 11, 13, 15, 17,
	5,   7,  9, 11, 13, 15, 17, 19,
	7,   9, 11, 13, 15, 17, 19, 21,
	9,  11, 13, 15, 17, 19, 21, 23,
	11, 13, 15, 17, 19, 21, 23, 25,
	13, 15, 17, 19, 21, 23, 25, 27,
	15, 17, 19, 21, 23, 25, 27, 29,
	17, 19, 21, 23, 25, 27, 29, 31
};

void UvlcVideoDecoder::blockBytes(short dataBlockBuffer[64], bool acCoefficientsAvailable)
{
	::memset(dataBlockBuffer, 0, 64 * sizeof(short));
	
	const unsigned int dcCoefficient = readStreamData(10);
	if(QuantizerMode != CONST_TableQuantization) {
		std::cout << "Constant quantizer mode is not yet implemented" << std::endl;
		return;
	}
	
	dataBlockBuffer[0] = (short)(dcCoefficient * quantizerValues[0]);
	
	if(!acCoefficientsAvailable) return;
	
	int run = 0;
	int level = 0;
	int zigZagPosition = 0;
	int matrixPosition = 0;
	bool last = false;
	
	decodeFieldBytes(run, level, last);
	
	while(!last) {
		zigZagPosition += run + 1;
		matrixPosition = zigZagPositions[zigZagPosition];
		level *= quantizerValues[matrixPosition];
		dataBlockBuffer[matrixPosition] = (short)level;
		decodeFieldBytes(run, level, last);
	}
}

bool UvlcVideoDecoder::readHeader(cv::Mat &image, MacroBlockVector &imageSlice)
{
	alignStreamData();
	
	unsigned int code = readStreamData(22);;
	unsigned int startCode = code & ~0x1F;
	
	if (startCode != UVLC_START_CODE) {
		std::cout << "Not a UVLC header!" << std::endl;
		return false;
	}
	
	if((code & 0x1F) == 0x1F) {
		PictureComplete = true;
		return true;
	}
	
	if(SliceIndex++ != 0) {
		QuantizerMode = readStreamData(5);
		return true;
	}
	
	PictureFormat = readStreamData(2);
	Resolution = readStreamData(3);
	PictureType = readStreamData(3);
	QuantizerMode = readStreamData(5);
	FrameIndex = readStreamData(32);
	
	unsigned width = 0;
	unsigned height = 0;
	switch (PictureFormat)
	{
	case kPictureFormatCIF:
		width = UVLC_CIF_WIDTH << Resolution - 1;
		height = UVLC_CIF_HEIGHT << Resolution - 1;
		break;
	case kPictureFormatVGA:
		width = UVLC_VGA_WIDTH << Resolution - 1;
		height = UVLC_VGA_HEIGHT << Resolution - 1;
		break;
	}
	
	const size_t blockCount = width >> 4;
	image = cv::Mat(height, width, CV_8UC3);
	if(imageSlice.size() != blockCount) imageSlice.resize(blockCount);
	
	return true;
}

void UvlcVideoDecoder::processStream(cv::Mat &image)
{
	// Set StreamFieldBitIndex to 32 to make sure that the first call to ReadStreamData 
	// actually consumes data from the stream
	StreamFieldBitIndex = 32;
	StreamField = 0;
	StreamIndex = 0;
	SliceIndex = 0;
	PictureComplete = false;
	
	MacroBlockVector imageSlice;
	short dataBlockBytes[64];
	
	while (!PictureComplete && StreamIndex < (ImageStreamLength >> 2)) {
		if(!readHeader(image, imageSlice)) return;
		
		if(PictureComplete) continue;
		
		MacroBlockVector::iterator it = imageSlice.begin();
		for(; it != imageSlice.end(); ++it) {
			unsigned int macroBlockEmpty = readStreamData(1);
			
			if(macroBlockEmpty) continue;
			
			const unsigned int acCoefficients = readStreamData(8);
			
			if(acCoefficients >> 6 & 1) {
				unsigned int quantizerMode = readStreamData(2);
				QuantizerMode = (int)((quantizerMode < 2) ? ~quantizerMode : quantizerMode);
			}
			
			MacroBlock &block = *it;
			
			blockBytes(dataBlockBytes, acCoefficients >> 0 & 1);
			inverseTransform(dataBlockBytes, block.dataBlocks[0]);
			
			blockBytes(dataBlockBytes, acCoefficients >> 1 & 1);
			inverseTransform(dataBlockBytes, block.dataBlocks[1]);
			
			blockBytes(dataBlockBytes, acCoefficients >> 2 & 1);
			inverseTransform(dataBlockBytes, block.dataBlocks[2]);
			
			blockBytes(dataBlockBytes, acCoefficients >> 3 & 1);
			inverseTransform(dataBlockBytes, block.dataBlocks[3]);
			
			blockBytes(dataBlockBytes, acCoefficients >> 4 & 1);
			inverseTransform(dataBlockBytes, block.dataBlocks[4]);
			
			blockBytes(dataBlockBytes, acCoefficients >> 5 & 1);
			inverseTransform(dataBlockBytes, block.dataBlocks[5]);
		}
		
		composeImageSlice(image, imageSlice);
	}
}

bool UvlcVideoDecoder::decode(const unsigned char *const buffer, const size_t length, cv::Mat &image)
{
	ImageStream = buffer;
	ImageStreamLength = length;
	processStream(image);
	ImageStream = 0;
	
	if(image.empty()) {
		std::cout << "UVLC decoder failed to decode incoming buffer." << std::endl;
		return false;
	}
	
	return true;
}