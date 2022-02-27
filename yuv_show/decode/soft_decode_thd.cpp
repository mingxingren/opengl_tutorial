//
// Created by MMK on 2021/9/1.
//

#include "soft_decode_thd.h"
#include <string>
#include <glad/glad.h>
#include "../render_test.hpp"

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

//    CRenderTest render_test(static_cast<SDL_Window*>(painter->GetWindow()));

    int32_t frame_nums = 0;
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
            frame_nums += 1;
            pPack->pts = frame_nums;
            printf("###############push packet pts: %d \n", pPack->pts);
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
                printf("#####################avcodec_receive_frame pts: %d  ret: %d\n", pFrameOrg->pts, ret);
                if (ret >= 0) {
                    ::sws_scale(m_pVideoSwsCtx, (const uint8_t * const*)pFrameOrg->data, pFrameOrg->linesize, 0,
                                pFrameOrg->height, rgb_frame->data, rgb_frame->linesize);
//                    std::cout << "#######################Get a frame " << av_get_pix_fmt_name(static_cast<AVPixelFormat>(rgb_frame->format)) << std::endl;
                    if (painter) {
                        if (m_eDstPixelFormat == AV_PIX_FMT_YUV420P) {
                            painter->PainterYUV(rgb_frame->width, rgb_frame->height, rgb_frame->data[0], rgb_frame->data[1], rgb_frame->data[2]);
                        }
                        else if (m_eDstPixelFormat == AV_PIX_FMT_NV12) {
                            painter->PainterNV(rgb_frame->width, rgb_frame->height, rgb_frame->data[0], rgb_frame->data[1]);
                        }
                        else{
                            painter->Painter(rgb_frame->data[0], rgb_frame->linesize[0], rgb_frame->width, rgb_frame->height);
                        }
                    }
//                    render_test.painter(rgb_frame->width, rgb_frame->height
//                                        , rgb_frame->data[0], rgb_frame->linesize[0]
//                                        , rgb_frame->data[1], rgb_frame->linesize[1]
//                                        , rgb_frame->data[2], rgb_frame->linesize[2]);
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

    // Always request low delay decoding
    m_pCodeContext->flags |= AV_CODEC_FLAG_LOW_DELAY;

    // Allow display of corrupt frames and frames missing references
    m_pCodeContext->flags |= AV_CODEC_FLAG_OUTPUT_CORRUPT;
    m_pCodeContext->flags2 |= AV_CODEC_FLAG2_SHOW_ALL;

    // Report decoding errors to allow us to request a key frame
    //
    // With HEVC streams, FFmpeg can drop a frame (hwaccel->start_frame() fails)
    // without telling us. Since we have an infinite GOP length, this causes artifacts
    // on screen that persist for a long time. It's easy to cause this condition
    // by using NVDEC and delaying 100 ms randomly in the render path so the decoder
    // runs out of output buffers.
    m_pCodeContext->err_recognition = AV_EF_EXPLODE;

    // Enable slice multi-threading for software decoding
    m_pCodeContext->thread_type = FF_THREAD_SLICE;
    printf("################SDL Get GPU numbers: %d \n", SDL_GetCPUCount());
    m_pCodeContext->thread_count = SDL_GetCPUCount();

//    m_pCodeContext->width = 2560;
//    m_pCodeContext->height = 1440;
//    m_pCodeContext->pix_fmt = AV_PIX_FMT_NV12;

    iRet = ::avcodec_open2(m_pCodeContext, pCodec, NULL);
    if (iRet < 0) {
        printf("avcodec_open2() failed, ret: %d", iRet);
        return;
    }

//    {
//        const uint8_t k_H264TestFrame[] = {
//                0x00, 0x00, 0x00, 0x01, 0x67, 0x64, 0x00, 0x20, 0xac, 0x2b, 0x40, 0x28, 0x02, 0xdd, 0x80, 0xb5, 0x06, 0x06, 0x06, 0xa5, 0x00, 0x00, 0x03, 0x03, 0xe8, 0x00, 0x01, 0xd4, 0xc0, 0x8f, 0x4a, 0xa0,
//                0x00, 0x00, 0x00, 0x01, 0x68, 0xee, 0x3c, 0xb0,
//                0x00, 0x00, 0x00, 0x01, 0x65, 0xb8, 0x02, 0x01, 0x67, 0x25, 0x1b, 0xf4, 0xfa, 0x7d, 0x40, 0x1a, 0x78, 0xe5, 0x10, 0x52, 0xc2, 0xee, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0xc6, 0xef, 0xbb, 0x81, 0x85, 0x2d, 0x47, 0xda, 0xca, 0x4c, 0x00, 0x00, 0x03, 0x00, 0x02, 0x7b, 0xcf, 0x80, 0x00, 0x45, 0x40, 0x01, 0x8d, 0xa6, 0x00, 0x01, 0x64, 0x00, 0x0e, 0x03, 0xc8, 0x00, 0x0e, 0x10, 0x00, 0xbd, 0xc5, 0x00, 0x01, 0x11, 0x00, 0x0e, 0xa3, 0x80, 0x00, 0x38, 0xa0,
//                0x00, 0x00, 0x01, 0x65, 0x00, 0x6e, 0x2e, 0x00, 0x83, 0x7f, 0xb4, 0x8e, 0x79, 0xa5, 0xff, 0x84, 0x3f, 0x7f, 0x34, 0x3f, 0x97, 0x1b, 0xaf, 0x31, 0x5f, 0x6e, 0xaa, 0xb6, 0xac, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x78, 0x36, 0x9d, 0xa4, 0x65, 0xf9, 0x1e, 0x5b, 0x3a, 0xb0, 0x40, 0x00, 0x00, 0x03, 0x00, 0x00, 0x41, 0x80, 0x00, 0xc5, 0xc8, 0x00, 0x00, 0xfa, 0x00, 0x03, 0x24, 0x00, 0x0e, 0x20, 0x00, 0x3f, 0x80, 0x01, 0x32, 0x00, 0x08, 0x68, 0x00, 0x3e, 0xc0, 0x03, 0x8a,
//                0x00, 0x00, 0x01, 0x65, 0x00, 0x37, 0x0b, 0x80, 0x21, 0x7f, 0xe3, 0x85, 0x1c, 0xd9, 0xff, 0xe1, 0x1b, 0x01, 0xfa, 0xc0, 0x3e, 0x11, 0x7e, 0xfe, 0x45, 0x5c, 0x43, 0x69, 0x31, 0x4b, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x02, 0x24, 0xae, 0x1a, 0xbb, 0xae, 0x75, 0x9e, 0x35, 0xae, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x03, 0x64, 0x00, 0x09, 0x98, 0x00, 0x1e, 0xc0, 0x00, 0x64, 0x80, 0x01, 0xc4, 0x00, 0x07, 0xf0, 0x00, 0x42, 0xf0, 0x00, 0x00, 0xe1, 0x98, 0x00, 0x05, 0x4b, 0x28, 0x00, 0x06, 0x04,
//                0x00, 0x00, 0x01, 0x65, 0x00, 0x14, 0xa2, 0xe0, 0x08, 0x5f, 0xe3, 0x85, 0x1c, 0xd9, 0xff, 0xe1, 0x1b, 0x01, 0xfa, 0xc0, 0x3e, 0x11, 0x7e, 0xfe, 0x45, 0x5c, 0x43, 0x69, 0x31, 0x4b, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x02, 0x24, 0xb9, 0x7d, 0xb4, 0x70, 0x4d, 0x15, 0xc5, 0x0a, 0x4e, 0x60, 0x00, 0x00, 0x03, 0x00, 0x00, 0x26, 0xa8, 0xb0, 0x00, 0x13, 0xcd, 0xcc, 0x00, 0x07, 0x48, 0x88, 0x00, 0x06, 0x29, 0x69, 0x00, 0x01, 0x16, 0xac, 0x80, 0x00, 0x9e, 0x4e, 0x80, 0x00, 0x3f, 0x84, 0x20, 0x00, 0x6f, 0x41, 0xa0, 0x00, 0x20, 0x00, 0x02, 0x16, 0xb8, 0x00, 0x08, 0x08
//        };
//
//        AVPacket* test_packet = av_packet_alloc();
//        test_packet->data = (uint8_t*)k_H264TestFrame;
//        test_packet->size = sizeof(k_H264TestFrame);
//
//        AVFrame* frame = av_frame_alloc();
//        int err;
//
//        // Some decoders won't output on the first frame, so we'll submit
//        // a few test frames if we get an EAGAIN error.
//        for (int retries = 0; retries < 5; retries++) {
//            // Most FFmpeg decoders process input using a "push" model.
//            // We'll see those fail here if the format is not supported.
//            err = avcodec_send_packet(m_pCodeContext, test_packet);
//            if (err < 0) {
//                av_frame_free(&frame);
//                char errorstring[512];
//                av_strerror(err, errorstring, sizeof(errorstring));
//                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
//                            "Test decode failed (avcodec_send_packet): %s", errorstring);
//                return;
//            }
//
//            // A few FFmpeg decoders (h264_mmal) process here using a "pull" model.
//            // Those decoders will fail here if the format is not supported.
//            err = avcodec_receive_frame(m_pCodeContext, frame);
//            if (err == AVERROR(EAGAIN)) {
//                // Wait a little while to let the hardware work
//                SDL_Delay(100);
//            }
//            else {
//                // Done!
//                break;
//            }
//        }
//
//        if (err < 0) {
//            char errorstring[512];
//            av_strerror(err, errorstring, sizeof(errorstring));
//            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
//                        "Test decode failed (avcodec_receive_frame): %s", errorstring);
//            av_frame_free(&frame);
//            return;
//        }
//
//        av_frame_free(&frame);
//    }

    m_pVideoSwsCtx = ::sws_getContext(m_pCodeContext->width, m_pCodeContext->height, m_pCodeContext->pix_fmt,
                                      m_pCodeContext->width, m_pCodeContext->height, m_eDstPixelFormat,
                                      SWS_BICUBIC, NULL, NULL, NULL);
}