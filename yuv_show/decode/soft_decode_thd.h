//
// Created by MMK on 2021/9/1.
//

#ifndef SOFT_DECODE_THD_H
#define SOFT_DECODE_THD_H

#include <array>
#include <thread>
#include <string>
extern "C"{
    #include <libavutil/channel_layout.h>
    #include <libavutil/opt.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
    #include <libavutil/imgutils.h>
}
#include "../painter.hpp"

class CSoftDecodeThd{
public:
    CSoftDecodeThd();
    ~CSoftDecodeThd() = default;
    void StartThd(const std::string &file_path, CPainter* painter);

protected:
    void run();

private:
    void _InitAudioFormat(AVFormatContext *_pAVFormatContext, int _iStreamIndex);
    /**
     * @brief _InitVideoFormat
     * @param _pAVFormatContext
     * @param _iStreamIndex
     * @param _pFrameOutRGB 给转码的目标图像分配内存
     * @param _pCodec 编码器
     */
    void _InitVideoFormat(AVFormatContext *_pAVFormatContext, int _iStreamIndex, AVFrame * _pFrameOutRGB, AVCodec * _pCodec);

private:
    struct SwrContext * m_pAudioSwrCtx = nullptr;
    AVCodecContext *m_pAudioCodeContext = nullptr;

    AVCodecContext *m_pCodeContext = nullptr;
    struct SwsContext * m_pVideoSwsCtx = nullptr;

private:
    std::thread *m_thread = nullptr;
    AVPixelFormat m_eDstPixelFormat = AV_PIX_FMT_NV12;
    std::string m_sFilePath;

    CPainter *painter = nullptr;
};

#endif //SOFT_DECODE_THD_H
