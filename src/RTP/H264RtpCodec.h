//
// Created by xzl on 2018/10/18.
//

#ifndef ZLMEDIAKIT_H264RTPCODEC_H
#define ZLMEDIAKIT_H264RTPCODEC_H

#include "RtpCodec.h"
#include "Util/ResourcePool.h"

using namespace ZL::Util;

/**
 * h264 rtp解码类
 */
class H264RtpDecoder : public RtpCodec {
public:
    H264RtpDecoder();
    ~H264RtpDecoder() {}

    /**
     * 输入264 rtp包
     * @param rtp rtp包
     * @param key_pos 此参数忽略之
     */
    void inputRtp(const RtpPacket::Ptr &rtp, bool key_pos = true) override;

    TrackType getTrackType() const override{
        return TrackVideo;
    }

    CodecId getCodecId() const override{
        return CodecH264;
    }
private:
    bool decodeRtp(const RtpPacket::Ptr &rtp);
    void onGetH264(const H264Frame::Ptr &frame);
    H264Frame::Ptr obtainFrame();
private:
    H264Frame::Ptr m_h264frame;
    ResourcePool<H264Frame> m_framePool;
};

/**
 * 264 rtp打包类
 */
class H264RtpEncoder : public H264RtpDecoder ,public RtpInfo{
public:

    /**
     * @param ui32Ssrc ssrc
     * @param ui32MtuSize mtu大小
     * @param ui32SampleRate 采样率，强制为90000
     * @param ui8PlayloadType pt类型
     * @param ui8Interleaved rtsp interleaved
     */
    H264RtpEncoder(uint32_t ui32Ssrc,
                   uint32_t ui32MtuSize = 1400,
                   uint32_t ui32SampleRate = 90000,
                   uint8_t ui8PlayloadType = 96,
                   uint8_t ui8Interleaved = TrackVideo * 2);
    ~H264RtpEncoder() {}

    /**
     * 输入264帧
     * @param frame 帧数据，必须
     * @param key_pos
     */
    void inputFrame(const Frame::Ptr &frame, bool key_pos) override;
private:
    void makeH264Rtp(const void *pData, unsigned int uiLen, bool bMark, uint32_t uiStamp);
private:
    unsigned char m_aucSectionBuf[1600];
};


#endif //ZLMEDIAKIT_H264RTPCODEC_H
