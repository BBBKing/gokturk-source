#include "../include/screen.h"

DesktopScreen::DesktopScreen()
    : m_Display(800, 500)
{
    m_Display.close();
    m_Display.set_title("Ekran Goruntusu");
}

DesktopScreen::~DesktopScreen()
{
    m_Display.flush();
}

void DesktopScreen::onShow()
{
    m_Display.show();
}

bool DesktopScreen::is_Closed()
{
    return m_Display.is_closed();
}

int DesktopScreen::renderDisplay(int compressSize, int width, int height, const char* imageBuffer )
{
 if(m_Display.is_closed() != true)
 {
    Compressor compressor1;
    AutoBuffer autoBuffer1(width*height*3);
    compressor1.decompress((Bytef*)imageBuffer, compressSize, (Bytef*)autoBuffer1.getPointer(), width*height*3);

    CImg<unsigned char> screenImage(width, height, 1, 3);

    int index =0;
    for (int x = 0; x < width; ++x){
        for (int y = 0; y < height ; ++y)
        {

                screenImage(x,y,0) = autoBuffer1[index];
                screenImage(x,y,1) = autoBuffer1[index+1];
                screenImage(x,y,2) = autoBuffer1[index+2];
            index+=3;
        }
    }

    m_Display.resize();
    m_Display.display(screenImage);

    if(m_Display.is_keyF1()){
        screenImage.save_png("./screen.png");
    }

 }
    return 0;
}

//WebCam!'^


CamScreen::CamScreen()
    : m_Display(640, 480)
{
    m_Display.close();
    m_Display.set_title("Web Kamera Goruntusu");
}

CamScreen::~CamScreen()
{
    m_Display.flush();
}

void CamScreen::onShow()
{
    m_Display.show();
}

bool CamScreen::is_Closed()
{
    return m_Display.is_closed();
}

int CamScreen::renderDisplay(const char* imageBuffer, size_t bufferSize )
{
 if(m_Display.is_closed() != true)
 {

    CImg<unsigned char> screenImage;
    screenImage.load_jpeg_buffer((JOCTET*)imageBuffer, bufferSize);

    m_Display.resize();
    m_Display.display(screenImage);

    if(m_Display.is_keyF1()){
        screenImage.save_png("./screen.png");
    }

 }
    return 0;
}


