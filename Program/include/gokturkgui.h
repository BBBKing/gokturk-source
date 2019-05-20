#ifndef GOKTURKGUI_H
#define GOKTURKGUI_H


#include <gtkmm.h>
#include <thread>
#include <fstream>


#include "../include/tcpserver.h"
#include "../include/packet.h"
#include "../include/screen.h"
#include "../include/microphone.h"

struct ConfigData{

    uint16_t port;
    char host[256];

};

class ModelColumns : public Gtk::TreeModel::ColumnRecord
{
    public:

        ModelColumns()
        {   add(m_id);
            add(m_username);
            add(m_computername);
            add(m_ipaddress);
            add(m_accesstype);
            add(m_desktoptype);}

        Gtk::TreeModelColumn<int>  m_id;
        Gtk::TreeModelColumn<Glib::ustring> m_username;
        Gtk::TreeModelColumn<Glib::ustring> m_computername;
        Gtk::TreeModelColumn<Glib::ustring> m_ipaddress;
        Gtk::TreeModelColumn<Glib::ustring> m_accesstype;
        Gtk::TreeModelColumn<Glib::ustring> m_desktoptype;
};



class GokturkGui : public Gtk::Window{
    private:
        TcpServer tcpServer1;
        bool m_Loop, m_Receiver, m_btn= true;
        std::mutex m_DataMutex;
        Glib::ustring savePth, localprocPath;

        DesktopScreen desktopScreen1;
        CamScreen camScreen1;


        SumPacket  sumScreenPacket1;
        SumPacket  sumCameraPacket1;
        SumPacket  sumMicroPacket1;
        SumPacket  sumFilePacket1;

    ScreenInfo screenInfo1{};
    CameraInfo cameraInfo1{};
    FileInfo   downFileInfo1{};
    FileInfo  upfileInfo1{};

    ScreenPacket screenPacket1{};
    CameraPacket cameraPacket1{};
    MicroPacket  microPacket1{};
    FilePacket   downFilePacket1{};

    public:

        GokturkGui(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
        void acceptThread();
        int  receiveThread(int selectId);
        int  sendFileThread(int selectId);
        int  getSelection();


    protected:

        ModelColumns m_Columns;
        ModelColumns m_Columns2;
        Glib::RefPtr<Gtk::Builder> builder;
        Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
        Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1;
//window1

        Gtk::Image *m_image1;
        Gtk::Window *m_window2;
        Gtk::Window *m_window4;
        Gtk::Window *m_window6;
        Gtk::Window *m_window7;
        Gtk::TreeView *m_treeview;

        Gtk::Button *m_setbtn;
        Gtk::Button *m_scrbtn;
        Gtk::Button *m_cambtn;
        Gtk::Button *m_micbtn;
        Gtk::Button *m_cmdbtn;
        Gtk::Button *m_crtbtn;
        Gtk::Button *m_aboutbtn;
        Gtk::LinkButton *m_linkbtn;

//window2
        Gtk::Button *m_startbtn;
        Gtk::Button *m_stopbtn;
        Gtk::Entry  *m_porttxt;
//window4
        Gtk::Entry  *m_cmdtxt;
        Gtk::TextView *m_cmdview;
//window6
        Gtk::Entry  *m_crthosttxt;
        Gtk::Entry  *m_crtporttxt;
        Gtk::Button *m_crtcreatebtn;

////////////////////////////
        void m_setbtn_clicked();
        void m_scrbtn_clicked();
        void m_cambtn_clicked();
        void m_micbtn_clicked();
        void m_cmdbtn_clicked();
        void m_crtbtn_clicked();
        void m_aboutbtn_clicked();

        void m_startbtn_clicked();
        void m_stopbtn_clicked();

        void m_crtcreatebtn_clicked();
//
        void command_txt_enter_key_press();

        int screenRequest(int selectID);
        int cameraRequest(int selectID);
        int microphomeRequest(int selectID);
};


#endif // GOKTURKGUI_H
