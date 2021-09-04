using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;


namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        unsafe struct dataFrame
        {

            public byte FrameType; // Says ACK or DataFrame 0->Dataframe , 1->ACK Frame
            public byte FLIGHT_STATUS; // send Coded info? 
                                       // 0 is WAITING
                                       // 1 is RISING 
                                       // 2    SEPERATING
                                       // 3    FLIGHTFALL
                                       // 4    PAYFALL
                                       // 5    FIXEDALT
                                       // 6    RESCUE

            public byte VIDEO_TRANSMISSION_INFO;
            // 0 is No
            // 1 is Yes


            public float altitude;   // Yukseklik
            public float pressure;   //Basınç
            public float temperature; //Sıcaklık

            public float turn_number; // Dönüş Sayısı
            public float pitch; // Pitch
            public float yaw; // Yaw
            public float roll; // Roll

            public float batteryVoltage;
            public float descentSpeed;

            public ushort Interval;
            public ushort TEAM_ID;
            public ushort package_number;

            public float latitude;
            public float longtitude;



            /*
             * 3 byte uint8_t (3 Count)
             * 6 byte uint16_t (3 Count)
             * 44 byte float   (11 count
             * += Total 53 byte
             */

        };

        //[StructLayout(LayoutKind.Sequential, Pack = 1)]
        unsafe struct gcsFrame
        {
            public byte tagType;
            /*
                M 88 5 11111
                0 -> N
                1 -> M
                2 -> VS
                3 -> V
            */
            public byte command;
            /*
                 88..
                 99..
                 77..
            */

            public byte bufferSize;      // For example video data is 11111 Buffer size will be = 5

            public fixed byte bufferArray[500]; // Max 500 Byte!


        };

        unsafe struct ackFrame
        {
            public byte FrameType; // should be 1
            public byte ACKType; 
            public byte ACK;
            /*

                ACKType:
                    0 -> VS   ACK
                    1 -> V    ACK
                    2 -> E    ACK (COMM ENDED)
                ACK :
                    0 -> UNSUCCESSFULLY
                    1 -> SUCCESSFULLY
                    2 -> if ACKType is  1 (V), Make ACK 2. Means Completed.
            */
        };


        enum PACKAGE_TYPE
        {   
            DATA_PACKAGE = 0 ,
            ACK_PACKAGE  = 1
        };

        dataFrame dataPacket;
        dataFrame unPackageData;

        gcsFrame gcsPacket; 

        byte[] packetArr;
        byte[] ackArr;
        public bool startedReading = false;
        public bool isReadingEnded = false;
        public int arrayCounter = 0;

        public bool isVideoEnabled = false;
        public int sendVideoByte = 0; // After vidoe sending process complete please make sure that value 
                                      // equals to 0 
        PACKAGE_TYPE packetType;

        public static int counterC = 0;

        public bool isPayloadCompleted = false;
        public bool isAckCompleted = false;

        public bool isPayloadReading = false;

        public byte ackFrameStartHeader  = 0xCC;
        public byte dataFrameStartHeader     = 0xBB;

        public static int packetArrayCt = 0;


        public Form1()
        {
            //packetArr = new byte[Marshal.SizeOf(dataPacket)]; // normally 53 byte
            packetArr   = new byte[54]; // normally 53 byte
            ackArr      = new byte[4];
            //for (int i = 0; i < 900; i ++)
            //{
            //    packetArr[i] = 0;
            //}
            InitializeComponent();
        }


        
        private void button1_Click(object sender, EventArgs e)
        {
            
            int size = Marshal.SizeOf(dataPacket);
            Console.WriteLine($"THE SIZE OF STRUCTURE {size}");
            dataPacket.FrameType = 0;
            dataPacket.FLIGHT_STATUS = 55;
            dataPacket.VIDEO_TRANSMISSION_INFO = 3;
            dataPacket.altitude = 1.9914F;
            dataPacket.pressure = 44.3f;

            dataPacket.latitude = 44;
            dataPacket.longtitude = 2;
            dataPacket.TEAM_ID = 27779;


            int sizeGcs = Marshal.SizeOf(gcsPacket);
            Console.WriteLine($"THE SIZE OF GroundStation Package {sizeGcs}");



            sendGCSFrame();
            
        }

        private void serialPort1_DataReceived(object sender,SerialDataReceivedEventArgs e)
        {
            Console.WriteLine("-------------------------------------");
            int bytesToRead = serialPort1.BytesToRead;
            Console.WriteLine($"Number of bytes in Serial Buffer {bytesToRead}");
            byte[] test = new byte[bytesToRead];
            int readedBytes = serialPort1.Read(test, 0, bytesToRead);
            Console.WriteLine(test[0]);
            if (test[0] == dataFrameStartHeader)
            {
                Console.WriteLine("DATA FRAME START HEADER..");
                if (!isPayloadReading) // Yarım kalan bir payload verisi falan degilse gelen veri (YARIM KALMIŞSA VE FRAME GELMISSE DIGERI KACMIS DEMEKTIR)
                {
                    isPayloadReading = true;
                    if (bytesToRead == 53)
                    {
                        isPayloadReading = false;
                    }
                    for (int i = 0; i < bytesToRead; i++)
                    {
                        if (test[i] != ackFrameStartHeader) // Guarantee  
                        {
                            packetArr[packetArrayCt++] = test[i];
                        }
                        else
                        {
                            if (i+2 > bytesToRead)
                            {
                                Console.WriteLine("error..");
                                isPayloadCompleted = false;
                                //packetArrayCt = 0;
                            }
                        }
                    }
                    if (!isPayloadReading && bytesToRead == 53) /*bunun Yerine yukarıda bytesToRead 53 ise isPayloadReading false cekmistik burda (!isPayloadReading && bytesToRead == 53) dyiebilriz belki
                                                                        packetArrayCt -1 == bytesToRead  */
                    {
                        Console.WriteLine("Tek seferde okumuş......");
                        packetArrayCt = 0;
                        isPayloadCompleted = true;
                    }
                }
                else
                {
                    Console.WriteLine("DAHA ONCE READ EDIYOMUŞSUN AMA YENISI GELMIS DIGER DATAN KAÇMIŞ KANKA..");
                }
            }
            else if (test[0] == ackFrameStartHeader)
            {
                Console.WriteLine("ACK FRAME START HEADER.....");
                for (int i = 0; i < 3; i++)
                {
                    ackArr[i] = test[i];
                }
                isAckCompleted = true;
                if (bytesToRead > 3) // Ack ile beraber  dataFrame gelmis demektir.
                {
                    isPayloadReading = true;
                    Console.WriteLine("ACK ile payload gelmis ");
                    for (int i = 3; i < bytesToRead; i++)
                    {
                        packetArr[packetArrayCt++] = test[i];

                    }
                    if (packetArrayCt == 53) // CT == 53
                    {
                        isPayloadCompleted = true; // if payload completed ise structure donustur dyiebiliriz
                        isPayloadReading = false;
                        packetArrayCt = 0;
                    }
                }

            }
            else
            {
                Console.WriteLine("Buyuk ihtimal bu package dataFrame'nin devami");
                if (isPayloadReading && test[0] != ackFrameStartHeader && test[0] != dataFrameStartHeader) // Geeln paket ne ack frame sine nede yeni data framesine eşitse ve
                                                                                                           //       payload read oluyosa bu yarım kalmış payload demektir                                                                                           // can we add !isPayloadCompleted    ?
                {
                    Console.WriteLine("dataFrame devami if Condition içindeuyiz..");
                    for (int i = 0; i < bytesToRead; i++)
                    {
                        packetArr[packetArrayCt++] = test[i];
                    }
                    if (packetArrayCt == 53)
                    {
                        Console.WriteLine($"Packet ARr Ct : {packetArrayCt}");
                        Console.WriteLine("yarım kalan paket tamamlandı..");
                        isPayloadReading = false;
                        isPayloadCompleted = true; // if payload completed ise structure donustur dyiebiliriz
                        packetArrayCt = 0; 
                    }
                }
            }

            if (isPayloadCompleted)
            {
                Console.WriteLine("Payload Copleted olmus. printliyorum..");
                for (int i = 0; i < 53; i++)
                {
                    Console.WriteLine($"{packetArr[i]}");
                }

                isPayloadCompleted = false;
                packetArrayCt = 0;
            }

            if (isAckCompleted)
            {
                Console.WriteLine("ACK COmnpleted olmuş printliyorum..");
                for (int i = 0; i < 3; i++)
                {
                    Console.WriteLine($"{ackArr[i]}");
                }
                isAckCompleted = false;
            }




            ////Console.WriteLine($"Readed Byte is {readedBytes}");
            //for (int i = 0; i < test.Length; i++)
            //{
            //    Console.WriteLine(test[i]);
            //}
            //Console.WriteLine("-------------------------------------");


            //int ALLOCATE_VAL = 0;
            //bool isValid = true;

            //Console.WriteLine($"Array'in 0.indexi bizim 8bitimiz 1 baytımız = {packetArr[0]}");
            //switch (packetArr[0])
            //{
            //    case (byte)PACKAGE_TYPE.DATA_PACKAGE:
            //        ALLOCATE_VAL = 53;
            //        Console.WriteLine("ESSEK");
            //        break;
            //    case (byte)PACKAGE_TYPE.ACK_PACKAGE:
            //        ALLOCATE_VAL = 3;
            //        Console.WriteLine("EŞŞOLUEŞŞEK");
            //        break;
            //    default:
            //        isValid = false;
            //        break;
            //}
            //if (isValid && (packetArr[0] == (byte)PACKAGE_TYPE.DATA_PACKAGE))
            //{
            //    IntPtr emptyPointer = Marshal.AllocHGlobal(ALLOCATE_VAL);
            //    Marshal.Copy(packetArr, 0, emptyPointer, readedBytes);
            //    // ACK Structure'ı gelmiş olabilir ona göre veri kontrolü sagla.
            //    unPackageData = (dataFrame)Marshal.PtrToStructure(emptyPointer, unPackageData.GetType());
            //    //Console.WriteLine($"Normally  Structure altitude {dataPacket.altitude}");
            //    Console.WriteLine($"Unpackage  Structure altitude {unPackageData.altitude}");
            //    Marshal.FreeHGlobal(emptyPointer);
            //}


        }


        public void sendGCSFrame() // This is example of sending GCSFrame Structure. (STRUCTURE TO BYTE ARRAY)
        {
            // Condition lazım , eğer video aktive ise video size'ı + datalar kadar yer ayırt etmeli
            // 

            int structureSize = Marshal.SizeOf(dataPacket); // We can give parameeter as a structureSize + sendVideoByte

            IntPtr ptr = Marshal.AllocHGlobal(structureSize + sendVideoByte); // Size verdik param
            byte[] byteA = new byte[structureSize + sendVideoByte];
            Marshal.StructureToPtr(dataPacket, ptr, true);
            Marshal.Copy(ptr, byteA, 0, structureSize + sendVideoByte);
            for (int i = 0; i < structureSize + sendVideoByte; i++)
            {
                Console.WriteLine(byteA[i]);
            }
            
            
            byte[] otherArray = new byte[4];
            for (int b = 3; b < 7; b++)
            {
                    otherArray[b - 3] = byteA[b];    
            }

            //serialPort1.Write(byteA, 0, byteA.Length); // Send buffer..

            IntPtr emptyPointer = Marshal.AllocHGlobal(byteA.Length);
            
            // ACK Structure'ı gelmiş olabilir ona göre veri kontrolü sagla.
            Marshal.Copy(byteA, 0, emptyPointer, byteA.Length);
            unPackageData = (dataFrame)Marshal.PtrToStructure(emptyPointer, unPackageData.GetType());
            Console.WriteLine($"Normally  Structure altitude {dataPacket.altitude}");
            Console.WriteLine($"Unpackage  Structure altitude {unPackageData.altitude}");
            Marshal.FreeHGlobal(emptyPointer);

            Console.WriteLine($"Array'in 0.indexi bizim 8bitimiz 1 baytımız = {byteA[0]}");
            switch (byteA[0])
            {
                case (byte)PACKAGE_TYPE.DATA_PACKAGE:
                    Console.WriteLine("ESSEK");
                    break;
                case (byte)PACKAGE_TYPE.ACK_PACKAGE:
                    Console.WriteLine("EŞŞOLUEŞŞEK");
                    break;
                default:
                    break;
            }

            float t = System.BitConverter.ToSingle(otherArray, 0);
            Console.WriteLine($"Float : : :  {t}");
            Marshal.FreeHGlobal(ptr); // Dont forget to make free pointer.
            

            return;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //serialPort1.ReceivedBytesThreshold = 53;
            serialPort1.Open();
            byte[] startWord = new byte[5];
            startWord[0] = 5;
            serialPort1.Write(startWord, 0, 1);
            Console.WriteLine(startWord[0]);
            //serialPort1.Write(startWord, 0, 1);
        }
    }
}
