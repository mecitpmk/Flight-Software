using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Diagnostics;
using System.Text.RegularExpressions;


namespace WinFormSerialing
{
    public partial class Form1 : Form
    {
        public String ReceivedString;
        public String ReceivedStringSAVED;
        public char[] MARKERS = { '>', '<' ,'\n'};
        public Regex pattern = new Regex("[<>]");
        
        public String COMMAND = "0";
        public List<String> COMMAND_QUEUE = new List<String>();
        
        
        public static bool VIDEO_SIZE_SENDED = false;
        public int VIDEO_SIZE = 10000; // 10 000 Byte

        public string VIDEO_BYTE = "11111"; // 5 Byte
        //public string VIDEO_BYTE = "1111111111111111111111111111111111111111"; // 50 Byte
        //public string VIDEO_BYTE = "11111111111111111111111111111111111111111111111111111111111111111111111111111111"; // 80 Byte
        //public string VIDEO_BYTE = "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"; // 100 Byte
        public static bool VIDEO_SENDING_ACTIVE = false;
        public static bool VIDEO_SENDING_COMPLETED = false;

        public static bool SENDING_PERMISSION = false;
        public static bool MissingTelemetry = false;



        public string[] Splitted_Telemetry;
        public Form1()
        {
            InitializeComponent();
            Console.WriteLine(VIDEO_SIZE);
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                PortLists.Items.Add(port);
            }
        }

       

        private void Connect_Event(object sender, EventArgs e)
        {
            serialPort1.PortName = PortLists.Text;
            serialPort1.BaudRate = 9600;
            serialPort1.Open();
            System.Threading.Thread.Sleep(200);
            Console.WriteLine("Sending Start Byte!");
            serialPort1.Write("<S>");
        }

        private void DataRECV_EVENT(object sender, SerialDataReceivedEventArgs e)
        {

            ReceivedString = serialPort1.ReadLine();
            ReceivedString = ReceivedString.Replace("\n", string.Empty).Replace("\r", string.Empty); // Removed "\n" , "\r"
            if (ReceivedString.StartsWith("<")) // Telemetri)
            {
                ReceivedString = pattern.Replace(ReceivedString, ""); // Removed  "<" , ">" 
                Console.WriteLine($"Telemetri came {ReceivedString}");
                Splitted_Telemetry = ReceivedString.Split(',');
                
                if (SENDING_PERMISSION && MissingTelemetry) // SENDING PERMISSION VARSA INTERVAL COKTAN GELMIS DEMEKTIR
                                                                // VE HATALI PAKET GELDI KI MISSINGTELEMETRY OLMUS BU YUZDEN INVOK EDEBEILIRIZ.
                {
                    this.Invoke(new EventHandler(checkTelemetries));
                }
                //checkTelemetries();

            }
            else if (ReceivedString.StartsWith("I"))
            {
                
                Console.WriteLine($"Interval Came! {ReceivedString}");
                this.Invoke(new EventHandler(SetInterval));
            }
            else if (ReceivedString.StartsWith("V"))
            {
                string[] splitted_STR = ReceivedString.Split(' ');
                if (splitted_STR[0] == "VS")
                {
                    Console.WriteLine($"VS BILGISI CAME {ReceivedString}");
                    String VS_REACHED = splitted_STR[1];
                    if (VS_REACHED == "1")
                    {
                        VIDEO_SIZE_SENDED = true; // SENDED SUCESSFULLY.
                        VIDEO_SENDING_ACTIVE = true;
                    }
                    //VIDEO_SEND_PROCESSING(); // IF SENDED OR NOT, DOESNT MATTER , GO TO THE FUNCTION,
                    // IF VS SENDED SEND VBINARY , IF NOT , SEND VS AGAIN.
                    this.Invoke(new EventHandler(VIDEO_SEND_PROCESSING));
                }
                else if (splitted_STR[0] == "V")
                {
                    Console.WriteLine(ReceivedString);
                    String V_REACHED = splitted_STR[1];
                    if (V_REACHED == "3")
                    {
                        VIDEO_SENDING_COMPLETED = true;
                        VIDEO_SENDING_ACTIVE = false;
                    }
                    else if (!VIDEO_SENDING_COMPLETED)
                    {
                        if (V_REACHED == "1")
                        {
                            /* 
                                    INCREASE INDEX OF ARRAY. YENI STRINGE ATA ARRAYDAKI VALUEYI
                                    ORNEK :  array = [1,1,1,1,0,1]
                                    VIDEO_BYTE =  array[FIRST_IDX : LAST_IDX].ToString() gibisinden..
                                */
                            int i = 0; // DELETE
                        }
                        // ELSE GEREK YOK ZATEN YUKARIDA IF'E GIREMDIGI ICIN INDEX FALN ARTMAYCAK.. OTOYINE
                        // AYNI VIDEO BYTE OLMUS OLACAK.
                        //VIDEO_SEND_PROCESSING();
                        this.Invoke(new EventHandler(VIDEO_SEND_PROCESSING));
                    }
                }
            }
            else
            {
                Console.WriteLine(ReceivedString);
            }
            
        }

        public void SetInterval(object sender , EventArgs e )
        {
            int INTERVAL = int.Parse(ReceivedString.Split(' ')[1].ToString());
            Console.WriteLine($"INTERVAL SETTED TO {INTERVAL-85}");
            PermissionTimer.Interval = INTERVAL-85;
            PermissionTimer.Stop();
            PermissionTimer.Start();
            SENDING_PERMISSION = true;
            Console.WriteLine("PERMISSION IS GIVEN...");
            this.Invoke(new EventHandler(checkTelemetries));
            //if (VIDEO_SENDING_ACTIVE && !VIDEO_SENDING_COMPLETED)
            //{
            //    this.Invoke(new EventHandler(VIDEO_SEND_PROCESSING));
            //}
        }
        public void checkTelemetries(object sender, EventArgs e) //(object sender, EventArgs e
        {
            //ReceivedStringSAVED = ReceivedString; // WE COPIED IT. (Another Event can be trigger Thats why.)
            //string[] Splitted_Telemetry = ReceivedStringSAVED.Split(',');
            if (Splitted_Telemetry.Length != 11)
            {
                MissingTelemetry = true;
                COMMAND_QUEUE_REQUEST();
                if (SENDING_PERMISSION) serialPort1.Write($"<M {COMMAND}>");
            }
            else
            {
                MissingTelemetry = false;
                TOTAL_PACK.Text = Splitted_Telemetry[1];
                TRANSFER_STATUS.Text = Splitted_Telemetry[10];
                Console.WriteLine("No missing Telemetries Checking videos and etc..");
                if (VIDEO_SENDING_ACTIVE && !VIDEO_SENDING_COMPLETED)
                {
                    Console.WriteLine("Videos IDENTIFIED ..");
                    this.Invoke(new EventHandler(VIDEO_SEND_PROCESSING));
                }
                else
                {
                    if (SENDING_PERMISSION)
                    {
                        COMMAND_QUEUE_REQUEST();
                        serialPort1.Write($"<N {COMMAND}>");
                    }
                }
            }
            

        }

        private void Manual_Motor_Always_Click(object sender, EventArgs e)
        {
            COMMAND_QUEUE.Add("99");
        }

        public void COMMAND_QUEUE_REQUEST()
        {
            if (COMMAND_QUEUE.Count() == 0)
            {
                COMMAND = "0";
            }
            else
            {
                COMMAND = COMMAND_QUEUE.ElementAt(0);
                COMMAND_QUEUE.Remove(COMMAND_QUEUE.ElementAt(0));
            }
        }

        private void TenSecond_Active_Click(object sender, EventArgs e)
        {
            COMMAND_QUEUE.Add("77");
        }

        private void Manual_Release_Click(object sender, EventArgs e)
        {
            COMMAND_QUEUE.Add("88");
        }

        private void SEND_VIDEO_Click(object sender, EventArgs e)
        {
            VIDEO_SENDING_ACTIVE = true;
            //VIDEO_SEND_PROCESSING();
            this.Invoke(new EventHandler(VIDEO_SEND_PROCESSING));
        }
        public void VIDEO_SEND_PROCESSING(object sender, EventArgs e)
        {
            COMMAND_QUEUE_REQUEST();
            if (!VIDEO_SIZE_SENDED && SENDING_PERMISSION)
            {

                Console.WriteLine($"<VS {COMMAND} {VIDEO_SIZE.ToString().Length} {VIDEO_SIZE}>");
                serialPort1.Write($"<VS {COMMAND} {VIDEO_SIZE.ToString().Length} {VIDEO_SIZE}>");
            }
            else
            {
                
                if (!VIDEO_SENDING_COMPLETED && SENDING_PERMISSION)
                {
                    Console.WriteLine($"<V {COMMAND} {VIDEO_BYTE.Length} {VIDEO_BYTE}>");
                    serialPort1.Write($"<V {COMMAND} {VIDEO_BYTE.Length} {VIDEO_BYTE}>");
                }
            }
        }

        private void Permission_OVER_EVENT(object sender, EventArgs e)
        {
            Console.WriteLine("PERMİSSİON OVER.");
            SENDING_PERMISSION = false;
            PermissionTimer.Stop();
            MissingTelemetry = false;
        }
    }
}
