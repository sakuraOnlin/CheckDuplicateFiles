#ifndef SHA1_H
#define SHA1_H

#include <cstdint>
#include <iostream>
#include <string>
#include "compute.h"

class SHA1 : public Compute
{
    Q_OBJECT

    static const size_t BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
    static const size_t BLOCK_BYTES = BLOCK_INTS * 4;
public:
    explicit SHA1(QObject *parent = 0);
    ~SHA1();

    void update(QString &s);
    QString getFinalResult();

public slots:
    void onStop();

private:
    void reset(uint32_t digest[], std::string &buffer, uint64_t &transforms);
    uint32_t rol(const uint32_t value, const size_t bits);
    uint32_t blk(const uint32_t block[BLOCK_INTS], const size_t i);
    void R0(const uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w,
            const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void R1(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w,
            const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void R2(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w,
            const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void R3(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w,
            const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void R4(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w,
            const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void transform(uint32_t digest[], uint32_t block[BLOCK_INTS], uint64_t &transforms);
    void buffer_to_block(const std::string &buffer, uint32_t block[BLOCK_INTS]);

private:
    bool m_isStart;
    uint32_t m_digest[5];
    uint64_t m_transforms;
    std::string m_buffer;

};

#endif // SHA1_H
