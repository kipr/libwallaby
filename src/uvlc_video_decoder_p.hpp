#ifndef _UVLC_VIDEO_DECODER_P_HPP_
#define _UVLC_VIDEO_DECODER_P_HPP_

#include <cstdlib>
#include <vector>
#include <opencv2/core/core.hpp>

#define UVLC_MAX_SIZE (921600)

namespace Private
{
	struct Rgb;
	
	struct MacroBlock
	{
		short dataBlocks[8][64]; 
	};

	typedef std::vector<MacroBlock> MacroBlockVector;
	
	class UvlcVideoDecoder
	{
	public:
		UvlcVideoDecoder();
		~UvlcVideoDecoder();
		
		bool decode(const unsigned char *const buffer, const size_t length, cv::Mat &image);
	private:
		void inverseTransform(short dataBlockBuffer[64], short *const block) const;
		int saturate5(int x);
		int saturate6(int x);
		void composeImageSlice(cv::Mat &image, const MacroBlockVector &imageSlice);
		void alignStreamData();
		int makeIntFromBytes(const unsigned char *const buffer, int index);
		unsigned int peekStreamData(const unsigned char *stream, int count);
		unsigned int readStreamData(int count);
		void decodeFieldBytes(int &run, int &level, bool &last);
		void blockBytes(short dataBlockBuffer[64], bool acCoefficientsAvailable);
		bool readHeader(cv::Mat &image, MacroBlockVector &imageSlice);
		void processStream(cv::Mat &image);

		unsigned int StreamField;
		int StreamFieldBitIndex;
		int StreamIndex;

		bool PictureComplete;

		int PictureFormat;
		int Resolution;
		int PictureType;
		int QuantizerMode;
		int FrameIndex;

		int SliceIndex;

		const unsigned char *ImageStream;
		size_t ImageStreamLength;
	};
}

#endif
