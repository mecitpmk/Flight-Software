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
using System.Threading;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public unsafe struct dataFrame
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
        public unsafe struct gcsFrame
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


            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 512)]
            //public byte [] bufferArray; // Max 500 Byte!


        };

        public byte [] videoBuffer = new byte[500];


        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public unsafe struct ackFrame
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


        public enum PACKAGE_TYPE
        {   
            DATA_PACKAGE = 0 ,
            ACK_PACKAGE  = 1,
        };

        public enum ACK_TYPE
        {
            videoSizeAck = 0,
            videoDataAck = 1,
            endSignalAck = 2,
            noneSignal   = 3
        }

        enum ACK_VALUES
        {
            videoDataFAILED    = 0,
            videoDataSUCCESS   = 1,
            videoDataENDED     = 2,
            endSignalEnabled   = 3,
            noneSignalEnabled  = 4
        }

        dataFrame   dataPacket;
        dataFrame   unPackageData;
        ackFrame    unpacageACK;
        gcsFrame    gcsPacket;
        ACK_TYPE    ackType;
        ACK_VALUES  ackValues;



        byte[] packetArr;
        byte[] ackArr;
        public bool startedReading = false;
        public bool isReadingEnded = false;
        public int arrayCounter    = 0;

        public bool isVideoEnabled      = false;
        public bool isVideoSizeSended   = false;
        public bool isVideoSended       = false;
        public int sendVideoByte        = 0; // After vidoe sending process complete please make sure that value 
                                                // equals to 0 
        PACKAGE_TYPE packetType;

        public static int counterC = 0;

        public bool isPayloadCompleted  = false;
        public bool isAckCompleted      = false;
        public bool sendingPermission   = false;
        public bool isPayloadReading    = false;
        public bool isAckReading        = false;

        public byte ackFrameStartHeader  = 0xCC;
        public byte dataFrameStartHeader = 0xBB;

        public static int packetArrayCt = 0;
        public static int ackArrayCt    = 0;


        public Form1()
        {
            //packetArr = new byte[Marshal.SizeOf(dataPacket)]; // normally 53 byte
            packetArr   = new byte[150]; // normally 53 byte
            ackArr      = new byte[4];

            InitializeComponent();


        }


        
        private void button1_Click(object sender, EventArgs e)
        {

            isVideoEnabled = true;

            Console.WriteLine("PRESSSED BUTTON");



            //int size = Marshal.SizeOf(dataPacket);
            //Console.WriteLine($"THE SIZE OF STRUCTURE {size}");
            //dataPacket.FrameType = 0xBB;
            //dataPacket.FLIGHT_STATUS = 3;
            //dataPacket.VIDEO_TRANSMISSION_INFO = 5;
            //dataPacket.altitude = 32.11f;
            //dataPacket.pressure = 11.333f;
            //dataPacket.temperature = 15;
            //dataPacket.turn_number = 1;

            //dataPacket.pitch = 7.11f;
            //dataPacket.yaw = -2.11f;
            //dataPacket.roll = 4.11f;
            //dataPacket.batteryVoltage = 3.33f;
            //dataPacket.descentSpeed = 0;
            //dataPacket.TEAM_ID = 1152;
            //dataPacket.package_number = 4;
            //dataPacket.latitude = 2;
            //dataPacket.longtitude = 5;
            //dataPacket.Interval = 700;
           

            //int sizeGcs = Marshal.SizeOf(gcsPacket);
            //Console.WriteLine($"THE SIZE OF GroundStation Package {sizeGcs}");
            //this.Invoke(new EventHandler(sendGCSFrame));
            
        

        
        }

        public unsafe void sendGCSFrameS(object sender , EventArgs e)
        {
            if (sendingPermission)
            {
                byte[] makedPackage = makeGCSPackage();
                serialPort1.Write(makedPackage, 0, makedPackage.Length);
                if (isVideoEnabled && !isVideoSended)
                {
                    Console.WriteLine("VIDEO ENABLEDMIS..");
                    serialPort1.Write(videoBuffer , 0, 100);
                    Console.WriteLine("video DAta sneded //////////////////////////////");
                }
                else if (isVideoSended)
                {
                    Console.WriteLine("******************Video COMPLETED 0*********************");
                }

                //for (int i = 0; i < makedPackage.Length; i++)
                //{
                //    Console.WriteLine(makedPackage[i]);
                //}
                Console.WriteLine("GCS Pkacage Sended..");
            }
            else
            {
                Console.WriteLine("PERMISSION IS NOT ENABLED..");
            }

        }

        public void ackPackageResponse(object sender , EventArgs e)

        {
            switch (unpacageACK.ACKType)
            {
                 case (byte)ACK_TYPE.videoSizeAck:
                    if (unpacageACK.ACK == (byte)ACK_VALUES.videoDataSUCCESS)
                    {
                        isVideoSizeSended = true;
                        ackValues = ACK_VALUES.videoDataSUCCESS;
                    }
                    else
                    {
                        ackValues = ACK_VALUES.videoDataFAILED;
                    }
                    break;

                case (byte)ACK_TYPE.videoDataAck:
                    if (unpacageACK.ACK == (byte)ACK_VALUES.videoDataSUCCESS)
                    {
                        Console.WriteLine("Video data sended go to next data..");
                        ackValues = ACK_VALUES.videoDataSUCCESS;
                    }
                    else
                    {
                        ackValues = ACK_VALUES.videoDataFAILED;
                    }
                    break;
                case (byte)ACK_TYPE.endSignalAck:
                    Console.WriteLine("Sending permission over..");
                    sendingPermission = false;
                    ackValues = ACK_VALUES.endSignalEnabled;
                    break;
                case (byte)ACK_TYPE.noneSignal:
                    Console.WriteLine("NONE SIGNAL CAME");
                    ackValues = ACK_VALUES.noneSignalEnabled;

                    break;
                default:
                    break;
            }

            switch (ackValues) // if ackValues 0 or 1 diyebiliriz
            {
                case ACK_VALUES.videoDataSUCCESS:
                    //if (isVideoSizeSended && !isVideoSended)
                    //{
                    //    // SendNewVideoData (incrase index and send new data)

                    //}
                    Console.WriteLine("video data sucesss");
                    //this.Invoke(new EventHandler(sendGCSFrameS));
                    break;

                case ACK_VALUES.videoDataFAILED:
                    //if (isVideoSizeSended)
                    //{
                    //    // Send VIDEO SIZE AGAIN!
                    //}
                    //else
                    //{
                    //    // SEND VIDEO DATA AGAIN! (dont increase index)
                    //}
                    Console.WriteLine("video data fail");

                    //this.Invoke(new EventHandler(sendGCSFrameS));
                    break;

                case ACK_VALUES.videoDataENDED:
                    isVideoSended = true;
                    break;

                case ACK_VALUES.endSignalEnabled:
                    Console.WriteLine("ACK VALUES PERMISSION FALSE!");
                    sendingPermission = false;
                    // permission is allready disabled.
                    break;
                case ACK_VALUES.noneSignalEnabled:
                    Console.WriteLine("NONE SIGNAL");
                    this.Invoke(new EventHandler(sendGCSFrameS));
                    break;
                default:
                    break;
            }
           
        }
        public void solveAckPackage(object sender, EventArgs e) // Burda gelen ACK Package'sini çözümlüyoruz. // object sender , EventArgs e
        {
            //Console.WriteLine($"simdi ACK Cozumluyoruz... {ackArr.Length}");
            IntPtr emptyPointer = Marshal.AllocHGlobal(ackArr.Length);
            //Console.WriteLine($"EMPTY POINTER ALLOCATED... SOLVE ACK PACKAGE");
            Marshal.Copy(ackArr, 0, emptyPointer, ackArr.Length);
            //Console.WriteLine($"COPIED QQQ... SOLVE ACK PACKAGE");
            unpacageACK = (ackFrame)Marshal.PtrToStructure(emptyPointer, unpacageACK.GetType());
            //Console.WriteLine($"NOW UNPKACKAGED...... SOLVE ACK PACKAGE");
            Marshal.FreeHGlobal(emptyPointer);
            //Console.WriteLine($"FREEING POINTER........ SOLVE ACK PACKAGE");

        }

        public void solveDataPackage(object sender, EventArgs e) //object sender , EventArgs e
        {
            Console.WriteLine("Data package is solving......");
            int lengthForPackage = 53;
            IntPtr emptyPointer = Marshal.AllocHGlobal(lengthForPackage);
            Marshal.Copy(packetArr, 0, emptyPointer, lengthForPackage);
            unPackageData = (dataFrame)Marshal.PtrToStructure(emptyPointer, unPackageData.GetType());
            //Console.WriteLine($"CONVERTED VALUE BEFORE FREE.. . . . . {unPackageData.pressure}");
            Marshal.FreeHGlobal(emptyPointer);
        }

        public unsafe byte [] makeGCSPackage() // GCS paketini hazırladık. Video enabled yada enabled olmama koşulunu da koyduk.
        {

            byte[] byteArr;
            if (isVideoEnabled && !isVideoSended)
            {
                if (!isVideoSizeSended)
                {
                    gcsPacket.tagType = 2;
                    gcsPacket.command = 0;
                    for (byte i = 0; i<5; i++)
                    {
                        videoBuffer[i] = i;
                    }
                    gcsPacket.bufferSize = 5;
                }
                else
                {
                    gcsPacket.tagType = 3;
                    gcsPacket.command = 0;
                    for (int i = 0; i < 100; i++)
                    {
                        videoBuffer[i] = 1;
                    }
                    gcsPacket.bufferSize = 5;
                }
                
                int structureSize = Marshal.SizeOf(gcsPacket);
                IntPtr ptr = Marshal.AllocHGlobal(structureSize); // Size verdik param
                byteArr = new byte[structureSize];
                Marshal.StructureToPtr(gcsPacket, ptr, true);
                Marshal.Copy(ptr, byteArr, 0, structureSize);
                Marshal.FreeHGlobal(ptr);

            }
            else
            {
                gcsPacket.tagType = 0;
                gcsPacket.command = 0;
                int structureSize = 2; // OnlyTagType and Command !
                IntPtr ptr = Marshal.AllocHGlobal(structureSize); // Size verdik param
                byteArr = new byte[structureSize];
                Marshal.StructureToPtr(gcsPacket, ptr, true);
                Marshal.Copy(ptr, byteArr, 0, structureSize);
                Marshal.FreeHGlobal(ptr);
            }
            return byteArr;
        }

        public void sendGCSFrame(object sender, EventArgs e) // This is example of sending GCSFrame Structure. (STRUCTURE TO BYTE ARRAY)
        {
           

            //int structureSize = Marshal.SizeOf(dataPacket); // We can give parameeter as a structureSize + sendVideoByte
            //IntPtr ptr = Marshal.AllocHGlobal(structureSize); // Size verdik param
            //byte[] byteA = new byte[structureSize ];
            //Marshal.StructureToPtr(dataPacket, ptr, true);
            //Marshal.Copy(ptr, byteA, 0, structureSize );
            //for (int i = 0; i < structureSize ; i++)
            //{
            //    Console.WriteLine(byteA[i]);
            //}


            //byte[] otherArray = new byte[4];
            //for (int b = 3; b < 7; b++)
            //{
            //    otherArray[b - 3] = byteA[b];
            //}


            //IntPtr emptyPointer = Marshal.AllocHGlobal(byteA.Length);

            //// ACK Structure'ı gelmiş olabilir ona göre veri kontrolü sagla.
            //Marshal.Copy(byteA, 0, emptyPointer, byteA.Length);
            //unPackageData = (dataFrame)Marshal.PtrToStructure(emptyPointer, unPackageData.GetType());
            //Console.WriteLine($"Normally  Structure altitude {dataPacket.altitude} , PRES {dataPacket.pressure}");
            //Console.WriteLine($"Unpackage  Structure altitude {unPackageData.altitude} , PRES {unPackageData.pressure} ");
            //Marshal.FreeHGlobal(emptyPointer);

            //Console.WriteLine($"Array'in 0.indexi bizim 8bitimiz 1 baytımız = {byteA[0]}");
            //switch (byteA[0])
            //{
            //    case (byte)PACKAGE_TYPE.DATA_PACKAGE:
            //        Console.WriteLine("ESSEK");
            //        break;
            //    case (byte)PACKAGE_TYPE.ACK_PACKAGE:
            //        Console.WriteLine("EŞŞOLUEŞŞEK");
            //        break;
            //    default:
            //        break;
            //}

            ////float t = System.BitConverter.ToSingle(otherArray, 0);
            ////Console.WriteLine($"Float : : :  {t}");
            //Marshal.FreeHGlobal(ptr); // Dont forget to make free pointer.


            return;
        }

        public void threadFunction()
        {
            while (true)
            {
                //Console.WriteLine("qq");
                if (serialPort1.BytesToRead > 0) // There is data to read.
                {
                    byte readedByte = (byte)serialPort1.ReadByte();
                    //Console.WriteLine($"Readed : {readedByte}");
                    if (readedByte == dataFrameStartHeader)
                    {
                        packetArrayCt               = 0;
                        isPayloadReading            = true;
                        isPayloadCompleted          = false;
                        
                        isAckReading                = false; // Sonradan added..
                        isAckCompleted              = false; // Sonradan added..
                        ackArrayCt                  = 0;

                        packetArr[packetArrayCt++]  = readedByte;
                        Console.WriteLine("DATA FRAME START CAME..");
                    }
                    else if (readedByte == ackFrameStartHeader)
                    {
                        Console.WriteLine("**************ACKFRAME*****************");
                        isAckReading         = true;
                        isAckCompleted       = false;
                        ackArrayCt           = 0;

                        isPayloadCompleted   = false; // Sonradan added..
                        isPayloadReading     = false; // Sonradan added..
                        packetArrayCt        = 0;


                        ackArr[ackArrayCt++] = readedByte;
                    }
                    else
                    {
                        if (isPayloadReading && !isAckReading && !isPayloadCompleted)
                        {
                            packetArr[packetArrayCt++] = readedByte;
                            if (packetArrayCt == 53)
                            {
                                packetArrayCt       = 0;
                                isPayloadReading    = false;
                                isPayloadCompleted  = true;
                                sendingPermission   = true;
                                Console.WriteLine("PAYLOAD COMPLETED OLMUŞ..");
                                //for (int i = 0; i<53; i++)
                                //{
                                //    Console.WriteLine(packetArr[i]);
                                //}
                                //solveDataPackage();

                                this.Invoke(new EventHandler(solveDataPackage));
                                Console.WriteLine($"Unpackaged data package_no : {unPackageData.package_number}");
                                this.Invoke(new EventHandler(sendGCSFrameS));
                                //Console.WriteLine($"Unpackaged data Altitude : {unPackageData.altitude}");
                            }
                        }
                        else if (!isPayloadReading && isAckReading && !isAckCompleted)
                        {
                            ackArr[ackArrayCt++] = readedByte;
                            if (ackArrayCt == 3)
                            {
                                ackArrayCt = 0;
                                isAckReading = false;
                                isAckCompleted = true;
                                Console.WriteLine("ACK COMPLETED OLMUŞ..");
                                //solveAckPackage();
                                this.Invoke(new EventHandler(solveAckPackage));
                                Console.WriteLine($"Ack Type : {unpacageACK.ACKType} , ACK  : {unpacageACK.ACK}");
                                this.Invoke(new EventHandler(ackPackageResponse));
                                //for (int i = 0; i < 3; i++)
                                //{
                                //    Console.WriteLine(ackArr[i]);
                                //}
                            }
                        }
                    }


                }
            }
            return;
        }
        private void button2_Click(object sender, EventArgs e)
        {
            //serialPort1.ReceivedBytesThreshold = 53;
            //serialPort1.Open();
            //byte[] startWord = new byte[5];
            //startWord[0] = 5;
            //serialPort1.Write(startWord, 0, 1);
            //Console.WriteLine(startWord[0]);

            serialPort1.Open();
            Thread t = new Thread(new ThreadStart(threadFunction));
            t.Start();
            //t.Join();


            //serialPort1.Write(startWord, 0, 1);
        }
    }
}
