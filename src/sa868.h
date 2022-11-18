
#ifndef SA868_H_
#define SA868_H_

#include "Stream.h"

typedef enum teCXCSS {
    //                  | Code Transform Bit Pattern | Hex Bit Pattern |
    E_CXCSS_NONE = 0,
    E_CXCSS_023I,    // | 11001000000111000110111    | 640E37          |
    E_CXCSS_025I,    // | 10101000000111101101011    | 540F6B          |
    E_CXCSS_026I,    // | 01101000000110111010011    | 340DD3          |
    E_CXCSS_031I,    // | 10011000000111111000101    | 4C0FC5          |
    E_CXCSS_032I,    // | 01011000000110101111101    | 2C0D7D          |
    E_CXCSS_043I,    // | 11000100000101101101101    | 620B6D          |
    E_CXCSS_047I,    // | 11100100000110111111000    | 720DF8          |
    E_CXCSS_051I,    // | 10010100000101010011111    | 4A0A9F          |
    E_CXCSS_054I,    // | 00110100000100101111011    | 1A097B          |
    E_CXCSS_065I,    // | 10101100000110001011101    | 560C5D          |
    E_CXCSS_071I,    // | 10011100000110011110011    | 4E0CF3          |
    E_CXCSS_072I,    // | 01011100000111001001011    | 2E0E4B          |
    E_CXCSS_073I,    // | 11011100000101100111010    | 6E0B3A          |
    E_CXCSS_074I,    // | 00111100000111100010111    | 1E0F17          |
    E_CXCSS_114I,    // | 00110010000101111010110    | 190BD6          |
    E_CXCSS_115I,    // | 10110010000111010100111    | 590EA7          |
    E_CXCSS_116I,    // | 01110010000110000011111    | 390C1F          |
    E_CXCSS_125I,    // | 10101010000111011110000    | 550EF0          |
    E_CXCSS_131I,    // | 10011010000111001011110    | 4D0E5E          |
    E_CXCSS_132I,    // | 01011010000110011100110    | 2D0CE6          |
    E_CXCSS_134I,    // | 00111010000110110111010    | 1D0DBA          |
    E_CXCSS_143I,    // | 11000110000101011110110    | 630AF6          |
    E_CXCSS_152I,    // | 01010110000100110111100    | 2B09BC          |
    E_CXCSS_155I,    // | 10110110000110110010001    | 5B0D91          |
    E_CXCSS_156I,    // | 01110110000111100101001    | 3B0F29          |
    E_CXCSS_162I,    // | 01001110000100111101011    | 2709EB          |
    E_CXCSS_165I,    // | 10101110000110111000110    | 570DC6          |
    E_CXCSS_172I,    // | 01011110000111111010000    | 2F0FD0          |
    E_CXCSS_174I,    // | 00111110000111010001100    | 1F0E8C          |
    E_CXCSS_205I,    // | 10100001000110010111011    | 508CBB          |
    E_CXCSS_223I,    // | 11001001000101110001011    | 648B8B          |
    E_CXCSS_226I,    // | 01101001000100001101111    | 34886F          |
    E_CXCSS_243I,    // | 11000101000111011010001    | 628ED1          |
    E_CXCSS_244I,    // | 00100101000101011111100    | 128AFC          |
    E_CXCSS_245I,    // | 10100101000111110001101    | 528F8D          |
    E_CXCSS_251I,    // | 10010101000111100100011    | 4A8F23          |
    E_CXCSS_261I,    // | 10001101000111101110100    | 468F74          |
    E_CXCSS_263I,    // | 11001101000100010111101    | 6688BD          |
    E_CXCSS_265I,    // | 10101101000100111100001    | 5689E1          |
    E_CXCSS_271I,    // | 10011101000100101001111    | 4E894F          |
    E_CXCSS_306I,    // | 01100011000111110011000    | 318F98          |
    E_CXCSS_311I,    // | 10010011000110110001110    | 498D8E          |
    E_CXCSS_315I,    // | 10110011000101100011011    | 598B1B          |
    E_CXCSS_331I,    // | 10011011000101111100010    | 4D8BE2          |
    E_CXCSS_343I,    // | 11000111000111101001010    | 638F4A          |
    E_CXCSS_346I,    // | 01100111000110010101110    | 338CAE          |
    E_CXCSS_351I,    // | 10010111000111010111000    | 4B8EB8          |
    E_CXCSS_364I,    // | 00101111000110100001011    | 178D0B          |
    E_CXCSS_365I,    // | 10101111000100001111010    | 57887A          |
    E_CXCSS_371I,    // | 10011111000100011010100    | 4F88D4          |
    E_CXCSS_411I,    // | 10010000100101101110111    | 484B77          |
    E_CXCSS_412I,    // | 01010000100100111001111    | 2849CF          |
    E_CXCSS_413I,    // | 11010000100110010111110    | 684CBE          |
    E_CXCSS_423I,    // | 11001000100110011101001    | 644CE9          |
    E_CXCSS_431I,    // | 10011000100110100011011    | 4C4D1B          |
    E_CXCSS_432I,    // | 01011000100111110100011    | 2C4FA3          |
    E_CXCSS_445I,    // | 10100100100100011101111    | 5248EF          |
    E_CXCSS_464I,    // | 00101100100101111110010    | 164BF2          |
    E_CXCSS_465I,    // | 10101100100111010000011    | 564E83          |
    E_CXCSS_466I,    // | 01101100100110000111011    | 364C3B          |
    E_CXCSS_503I,    // | 11000010100101100011110    | 614B1E          |
    E_CXCSS_506I,    // | 01100010100100011111010    | 3148FA          |
    E_CXCSS_516I,    // | 01110010100111011000001    | 394EC1          |
    E_CXCSS_532I,    // | 01011010100111000111000    | 2D4E38          |
    E_CXCSS_546I,    // | 01100110100101111001100    | 334BCC          |
    E_CXCSS_565I,    // | 10101110100111100011000    | 574F18          |
    E_CXCSS_606I,    // | 01100001100110011011101    | 30CCDD          |
    E_CXCSS_612I,    // | 01010001100110001110011    | 28CC73          |
    E_CXCSS_624I,    // | 00101001100110101111000    | 14CD78          |
    E_CXCSS_627I,    // | 11101001100111111000000    | 74CFC0          |
    E_CXCSS_631I,    // | 10011001100100010100111    | 4CC8A7          |
    E_CXCSS_632I,    // | 01011001100101000011111    | 2CCA1F          |
    E_CXCSS_654I,    // | 00110101100111000011001    | 1ACE19          |
    E_CXCSS_662I,    // | 01001101100111100010010    | 26CF12          |
    E_CXCSS_664I,    // | 00101101100111001001110    | 16CE4E          |
    E_CXCSS_703I,    // | 11000011100111010100010    | 61CEA2          |
    E_CXCSS_712I,    // | 01010011100110111101000    | 29CDE8          |
    E_CXCSS_723I,    // | 11001011100100011001110    | 65C8CE          |
    E_CXCSS_731I,    // | 10011011100100100111100    | 4DC93C          |
    E_CXCSS_732I,    // | 01011011100101110000100    | 2DCB84          |
    E_CXCSS_734I,    // | 00111011100101011011000    | 1DCAD8          |
    E_CXCSS_743I,    // | 11000111100110110010100    | 63CD94          |
    E_CXCSS_754I,    // | 00110111100111110000010    | 1BCF82          |
    E_CXCSS_0001,    // 67
    E_CXCSS_0002,    // 71.9
    E_CXCSS_0003,    // 74.4
    E_CXCSS_0004,    // 77
    E_CXCSS_0005,    // 79.7
    E_CXCSS_0006,    // 82.5
    E_CXCSS_0007,    // 85.4
    E_CXCSS_0008,    // 88.5
    E_CXCSS_0009,    // 91.5
    E_CXCSS_0010,    // 94.8
    E_CXCSS_0011,    // 97.4
    E_CXCSS_0012,    // 97.4
    E_CXCSS_0013,    // 103.5
    E_CXCSS_0014,    // 107.2
    E_CXCSS_0015,    // 110.9
    E_CXCSS_0016,    // 114.8
    E_CXCSS_0017,    // 118.8
    E_CXCSS_0018,    // 123
    E_CXCSS_0019,    // 127.3
    E_CXCSS_0020,    // 131.8
    E_CXCSS_0021,    // 136.5
    E_CXCSS_0022,    // 141.3
    E_CXCSS_0023,    // 146.2
    E_CXCSS_0024,    // 151.4
    E_CXCSS_0025,    // 156.7
    E_CXCSS_0026,    // 162.2
    E_CXCSS_0027,    // 167.9
    E_CXCSS_0028,    // 173.8
    E_CXCSS_0029,    // 179.9
    E_CXCSS_0030,    // 186.2
    E_CXCSS_0031,    // 192.8
    E_CXCSS_0032,    // 203.5
    E_CXCSS_0033,    // 210.7
    E_CXCSS_0034,    // 218.1
    E_CXCSS_0035,    // 225.7
    E_CXCSS_0036,    // 233.6
    E_CXCSS_0037,    // 241.8
    E_CXCSS_0038,    // 250.3
} teCXCSS;

const char cxcss[][5] = {
    "0000",
    "023I", "025I", "026I", "031I", "032I", "043I", "047I", "051I",
    "054I", "065I", "071I", "072I", "073I", "074I", "114I", "115I",
    "116I", "125I", "131I", "132I", "134I", "143I", "152I", "155I",
    "156I", "162I", "165I", "172I", "174I", "205I", "223I", "226I",
    "243I", "244I", "245I", "251I", "261I", "263I", "265I", "271I",
    "306I", "311I", "315I", "331I", "343I", "346I", "351I", "364I",
    "365I", "371I", "411I", "412I", "413I", "423I", "431I", "432I",
    "445I", "464I", "465I", "466I", "503I", "506I", "516I", "532I",
    "546I", "565I", "606I", "612I", "624I", "627I", "631I", "632I",
    "654I", "662I", "664I", "703I", "712I", "723I", "731I", "732I",
    "734I", "743I", "754I",
    "0001", "0002", "0003", "0004", "0005", "0006", "0007", "0008",
    "0009", "0010", "0011", "0012", "0013", "0014", "0015", "0016",
    "0017", "0018", "0019", "0020", "0021", "0022", "0023", "0024",
    "0025", "0026", "0027", "0028", "0029", "0030", "0031", "0032",
    "0033", "0034", "0035", "0036", "0037", "0038"
};

class SA868 {
public:
    /**
     * constructor
     *
     * @param[in] pttPin 发射/接收控制脚， “0” 为发射； “1” 为接收， 默认接收模式
     * @param[in] pdPin 模块休眠使能， “0” 或者悬空为休眠； “1” 为正常工作
     * @param[in] rfPin 高低功率控制； 接地为低功率， 悬空为高功率， 默认高功率
     */
    SA868(Stream& stream, uint8_t pttPin, uint8_t pdPin, uint8_t rfPin);
    ~SA868();

    void begin();

    void sleep();

    void wake();
    /**
     * receive mode
     */
    void receive();

    void highPower();

    void lowPower();

    /**
     * transmission mode
     */
    void transmit();

    /**
     * Set the frequency to scan
     * 设置要扫描的频率
     *
     * @param[in] freq frequency to scan
     *
     * @return
     * - true signal
     * - false No signal or command error
     */
    bool scanRF(double freq);

    /**
     * Set working parameters
     * 设置模块的工作参数
     *
     * @param[in] bandwidth bandwidth
     * @param[in] transFreq transmit frequency
     * @param[in] recvFreq  receive frequency
     * @param[in] txCXCSS   transmit CXCSS
     * @param[in] sq        Background noise(0～8) （ 0： 监听模式， 扫描模式不能使用 0）
     * @param[in] rxCXCSS   receive CXCSS
     *
     * @return
     * - true success
     * - false Data setting out of range
     */
    bool setGroup(bool      bandwidth,
                  long long transFreq,
                  long long recvFreq,
                  teCXCSS   txCXCSS,
                  uint8_t   sq,
                  teCXCSS   rxCXCSS);

    /**
     * Set volume level
     * 设置模块的音量级别
     *
     * @param[in] volume level
     *
     * @return
     * - true success
     * - false fail
     */
    bool setVolume(uint8_t volume);

    /**
     * get signal strength
     * 读取模块的信号强度
     */
    int getRSSI();

    /**
     * set filter
     * 设置模块的滤波器
     * @param[in] emphasis PRE/DE-EMPH,为 0 表示预加重/去加重打开， 为 1 表示预加重/去加重关闭；
     * @param[in] highPass 为 0 表示高通滤波器打开， 为 1 表示高通滤波器关闭
     * @param[in] lowPass 为 0 表示低通滤波器打开， 为 1 表示低通滤波器关闭
     *
     * @return
     * - true success
     * - false fail
     */
    bool setFilter(bool emphasis, bool highPass, bool lowPass);

    bool checkFreq(bool bandwidth, long long freq);

    bool       bandwidth;
    long long  transFreq;
    long long  recvFreq;
    int        txCXCSS;
    uint8_t    sq;
    int        rxCXCSS;
    uint8_t    volume;
    int        rssi;
    bool       emphasis;
    bool       highPass;
    bool       lowPass;

private:
    bool waitResponse(String& data, String rsp = "\r\n", uint32_t timeout = 1000);

protected:
    Stream *_stream;
    uint8_t _pttPin = 0;
    uint8_t _pdPin  = 0;
    uint8_t _rfPin  = 0;
    bool _transmitStatus = false;
};


#endif
