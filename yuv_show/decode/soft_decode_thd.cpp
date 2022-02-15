//
// Created by MMK on 2021/9/1.
//

#include "soft_decode_thd.h"
#include <string>
#include <glad/glad.h>

CSoftDecodeThd::CSoftDecodeThd() {

}

void CSoftDecodeThd::StartThd(const std::string &file_path, CPainter* painter){
    if (painter != nullptr) {
        this->painter = painter;
    }

    if (this->m_thread == nullptr) {
        this->m_sFilePath = file_path;
        this->m_thread = new std::thread(&CSoftDecodeThd::run, this);
    }
}

void CSoftDecodeThd::run(){
    AVFormatContext *pFormatContex = nullptr;
    int iRet = ::avformat_open_input(&pFormatContex, this->m_sFilePath.c_str(), NULL, NULL);
    if (!pFormatContex)
    {
        printf("avformat_open_input fail \n", "");
        return;
    }
    printf("avformat_open_input success \n");

    if (avformat_find_stream_info(pFormatContex, NULL) < 0)
    {
        printf("could not find stream information \n", "");
        return;
    }

    printf("avformat_find_stream_info success \n", "");
    av_dump_format(pFormatContex, 0, this->m_sFilePath.c_str(), 0);

    int iAudioStreamIndex = -1;
    int iVideoStreamIndex = -1;

    double dAudioUnit = 0, dVieoUnit = 0;
    AVCodec *pVideoDecoder = nullptr;

    for (uint32_t i = 0; i < pFormatContex->nb_streams; i++)
    {
        if (pFormatContex->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            iAudioStreamIndex = i;
            dAudioUnit = av_q2d(pFormatContex->streams[i]->time_base) * 1000;
            printf("Find a audio stream, index : %d, Time Unit: %f", iAudioStreamIndex, dAudioUnit);
        }

       if (pFormatContex->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
       {
            iVideoStreamIndex = i;
            dVieoUnit = av_q2d(pFormatContex->streams[i]->time_base) * 1000;
            printf("Find a video stream, index : %d, Time Unit: %f", iVideoStreamIndex, dVieoUnit);
       }
    }

    uint8_t * pAudioBuffer = nullptr;
    int iAudioBufferSize;
    if (iAudioStreamIndex == -1)
    {
        printf("Cant find audio stream \n");
    }
    else{
        this->_InitAudioFormat(pFormatContex, iAudioStreamIndex);
    }

    AVFrame * pVideoFrameRGB = av_frame_alloc();
    if (iVideoStreamIndex == -1)
    {
        printf("Cant find video stream \n");
    }
    else{
        this->_InitVideoFormat(pFormatContex, iVideoStreamIndex, pVideoFrameRGB, pVideoDecoder);
    }
    unsigned int iCurrentAudioBuffer = 0;
    int iCurrentTargetSample = 0;
    AVPacket *pPack = av_packet_alloc();

    std::shared_ptr<AVFrame> rgb_frame(av_frame_alloc(), [](AVFrame* inner){
        av_frame_free(&inner);
    });
    rgb_frame->format = this->m_eDstPixelFormat;
    rgb_frame->width = m_pCodeContext->width;
    rgb_frame->height = m_pCodeContext->height;
    av_frame_get_buffer(rgb_frame.get(), 1);

    if (painter) {
        std::cout << "########################painter->MakeCurrentContext" << std::endl;
        painter->MakeCurrentContext();
    }

    while (true) {
        ::av_packet_unref(pPack);
        if (::av_read_frame(pFormatContex, pPack) != 0){
            break;
        }

        // 解析出视频格式
        if (pPack->stream_index == iAudioStreamIndex){

        }
        else if(pPack->stream_index == iVideoStreamIndex)
        {
            iRet = ::avcodec_send_packet(m_pCodeContext, pPack);
            if (iRet != 0)
            {
                printf("avcodec_receive_frame() failed: %d \n", iRet);
                return;
            }

            int ret = 0;
            do {
                std::shared_ptr<AVFrame> pFrameOrg(av_frame_alloc(), [](AVFrame* p){
                    av_frame_free(&p);
                });
                ret = ::avcodec_receive_frame(m_pCodeContext, pFrameOrg.get());
                if (ret >= 0) {
                    ::sws_scale(m_pVideoSwsCtx, (const uint8_t * const*)pFrameOrg->data, pFrameOrg->linesize, 0,
                                pFrameOrg->height, rgb_frame->data, rgb_frame->linesize);
                    std::cout << "#######################Get a frame" << std::endl;
                    if (painter) {
                        painter->Painter(rgb_frame->data[0], rgb_frame->linesize[0], rgb_frame->width, rgb_frame->height);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            } while (ret != AVERROR(EAGAIN));
        }
    }

    printf("####################_End Decode \n");

    ::av_packet_free(&pPack);

    ::av_free(pAudioBuffer);

    ::swr_free(&m_pAudioSwrCtx);

    ::avcodec_close(m_pAudioCodeContext);
    ::avcodec_free_context(&m_pAudioCodeContext);

    ::avformat_close_input(&pFormatContex);
}

void CSoftDecodeThd::_InitAudioFormat(AVFormatContext *_pAVFormatContext, int _iStreamIndex) {

}

void CSoftDecodeThd::_InitVideoFormat(AVFormatContext *_pAVFormatContext, int _iStreamIndex,
                                  AVFrame *pFrameOutRGB, AVCodec * _pCodec) {
    AVCodecParameters *pCodeParam = _pAVFormatContext->streams[_iStreamIndex]->codecpar;

    const AVCodec *pCodec = ::avcodec_find_decoder(pCodeParam->codec_id);
    if (pCodec == NULL) {
        printf("Cannt find video codec!\n");
        return;
    }

    m_pCodeContext = ::avcodec_alloc_context3(pCodec);
    int iRet = ::avcodec_parameters_to_context(m_pCodeContext, pCodeParam);
    if (iRet < 0) {
        printf("avcodec_parameters_to_context() failed, ret: %d", iRet);
        return;
    }

    iRet = ::avcodec_open2(m_pCodeContext, pCodec, NULL);
    if (iRet < 0) {
        printf("avcodec_open2() failed, ret: %d", iRet);
        return;
    }
    m_pVideoSwsCtx = ::sws_getContext(m_pCodeContext->width, m_pCodeContext->height, m_pCodeContext->pix_fmt,
                                      m_pCodeContext->width, m_pCodeContext->height, m_eDstPixelFormat,
                                      SWS_BICUBIC, NULL, NULL, NULL);
}