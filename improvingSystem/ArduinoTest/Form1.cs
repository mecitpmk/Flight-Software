using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using System.IO.Ports;
using System.Diagnostics;



/*

    Fikir 1:
    Şöyle bi şey düşündüm.
    Eksik data  var ve alamadık zamanında, kalman filtresi uygulayalım. peki kalman filtresi nasıl çalışacak?
    Time'a bağlı odluğu kesin yani bir time counter'imiz olsun her saniye+1 lesin kendini...
    Yani gelen datayı listeye biz (dataval,time) olarak atalım.

    Daha sonra ekisk value'leri check ederken, time'a bağlı olarak buluruz belki bu şu anlık fikir olarak dursun.
    
    Yapılacaklar:
        Plot etmeyi araştır. 
            Dataları List<> olarak topluyorum direkt DataSource olarak veriliyor mu check et
               Hallet onu.
    

*/
namespace ArduinoTest
{
    public partial class Form1 : Form
    {
        static bool clickedStartBefore = false;
        public string data;


        public List<int> missed_list = new List<int>();
        public List<int> reached_list = new List<int>();

        public List<int> list_range = (from i in Enumerable.Range(1, 11 - 1) select i).ToList();
        public Dictionary<int, List<object>> my_dict = Enumerable.Range(1, 11 - 1).ToDictionary(i => i, i => new List<object>());
        public List<int> command_list = new List<int>();

        public List<int> test_list_graph = new List<int>();

        public Dictionary<int, System.Windows.Forms.DataVisualization.Charting.Chart> graphDict = new Dictionary<int, System.Windows.Forms.DataVisualization.Charting.Chart>();
        public string[] SatStatus = { "WAITING", "RISING", "SEPERATING", "FALLING", "PAYLFALL", "RESCUE" };

        public List<object> graphList = new List<object>();

        public List<int> my_misseds = new List<int>();

        // 11111 degil de daha da dusur videoBinary cpp dosyasında BufferIndex'ı kücült.
        public string videoBinary = "11111"; // 24 byte took only 49 package :D ? This is the best for now.
        //public string videoBinary = "111111111111111111111111"; // 24 byte
        //public string videoBinary = "111111111111111111111111"; // 24 byte  56 Package tooks only   
        //public string videoBinary = "1111111111111111111111111111111111111111"; // 40 byte //This is in 900 ms Interval FOR REAL 71 package
        //public string videoBinary = "11111111111111111111111111111111111111111111111111111111111111111111111111111111"; // 95 package 
        //public string videoBinary = "FFFFFFFFFFFFFFFFFFFFFFFFFFCCFCFFCFAFFDDDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; // 83 package

        //public string videoBinary = "AFDFDFDFDFDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFCCFCFFCFAFFDDDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1110"; // 24 byte
        //public string videoBinary = "FFFFFFFFFFFFFFFFFFFFFFFFFFCCFCFFCFAFFDDDFFFFFFFFFFFFFFFFF"; // 61 This is worked in
        // Date of 3/12/2021 it tooks 80 sec for Interval 900ms Permission.

        //public string videoBinary = "AFDFDFDFDFDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFCCFCFFCFAFFDDDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1110AFDFDFDFDFDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFCCFC";
        public bool missedDataAvailable = false;
        public bool motorActive10sec = false;
        public bool motorActiveAlways = false;
        public bool sendReleaseCommand = false;

        public bool sendingVideoEnabled = false;
        

        public bool changedColorStatus = false;
        public bool errorCatched = false;

        //public bool sendingPermisson = false; // Normally that is the truth values
        public bool sendingPermisson = true; // This is just for testing purposes.

        public bool isTimedOut = false;
        public bool isTimerOnlineBefore = false;
        public bool missedisDone = false;
        public bool isNewData = false;
        public int currentlyMissed = 0;
        public bool samePackage = false;
        public bool isTimerActivatedBefore = false;
        public bool isSendedVideoSize = false;
        public int totalVideoSize = 2048;
        public bool isVideoSizeReached = false;


        public int currentTelemetry;
        public int lastReceivedPackageNo = 0;


        public int transferredBytes = 0;
        public int first = 0;
        public object second = null;
        public static Stopwatch MyStopWatchTimer = new Stopwatch();
        public static Stopwatch IntervalCalculator = new Stopwatch();
        public double beforeTime;
        public double afterTime;
        public double RemainTime;
        public double tookTime;
        public bool FirstCalculatingElapsed = true;
        
        

        public int Row = 0;
        public int RowAdder = 1;
        public int packageCounter = -1;


        public string[] beforeJumped;
        public List<int> temporary_reached = new List<int>();
        public List<int> temporary_missed = new List<int>();

        public Form1()
        {
            InitializeComponent();
        }




        private void Form1_Load(object sender, EventArgs e)
        {
            

            //graphList.Add(packGraph);
            //List<int> testing_list = new List<int>();
            //testing_list.Add(1);
            //Console.WriteLine($"Count is {testing_list.Count()} and Val : {testing_list[testing_list.Count()-1]}");

            //timer3.Interval = 9999999;
            timer3.Interval = 900;
            string simple_string = "1 555,2 666,3 215";
            string[] spl = simple_string.Split(',');
            foreach(var ix in spl)
            {
                string[] asdf = ix.Split(' ');
                string fff = asdf[0];
                string sss = asdf[1];
                Console.WriteLine($" first {fff} sec {sss}");
            }

            // Clear List Before Start
            reached_list.Clear();
            missed_list.Clear();


            collectInDictGrph(); //Add Graph to dictionary
            timer2.Start(); // Status Color blink.


            string[] ports = SerialPort.GetPortNames();

            //list_range.ForEach(Console.WriteLine);


            foreach (string port in ports)
            {
                portCombo.Items.Add(port);
            }
            //portCombo.SelectedIndex = 0; // Uncommentle 
        }




        public void comingDataFunct(object sender , EventArgs e)
        {

            if (missedDataAvailable && my_misseds.Contains(first) && !reached_list.Contains(first))
            {
                Console.WriteLine($"Misse Data Comed... {first} - {second}");
                my_misseds.Remove(first);
                if (my_misseds.Count() == 0)
                {
                    missedDataAvailable = false;
                    missedisDone = true;
                }
            }

            if (reached_list.Contains(first) && !my_misseds.Contains(first) && !missedDataAvailable)
            {
                //Console.WriteLine("Data is {first}")
                Console.WriteLine($"Telemetry is {first} Data : {second}");
                //this.Invoke(new EventHandler(forgridViewing));
            }

            //if (!missedDataAvailable)
            //{ 
            //   Console.WriteLine($"Telemetry is {first} Data : {second}");
            //}


            //Console.WriteLine($"Before Adding values are {first} and {second}");

            currentTelemetry = first;

            if (first == 2 || first == 1)
            {
                if (first == 1)
                {
                    reached_list.Add(first);
                }
                if (!my_dict[first].Contains(second))
                {
                    samePackage = false;
                }
                else
                {
                    samePackage = true;
                }
            }
            if (!samePackage)
            {
                reached_list.Add(first);
                my_dict[first].Add(second);
                samePackage = false;
            }
            //reached_list.Add(first);
            //my_dict[first].Add(second);


            // [ 150 250 300]
            // []

            // Try-catch kullan
            // eski = my_dict[first][ my_dict[first].Count() -1 ];
            // second -> yeni 
            // if (eski != second)
            // add



            if ((first == 2 || first == 3 || first == 4 || first == 5) && !samePackage)
            {
                
                this.Invoke(new EventHandler(findGraph));
                
                
                //findGraph(first);
            }
            if (first == 6)
            {
                statusLabel.Text = second.ToString();
            }

            //if (missedDataAvailable && my_misseds.Count() > 0 && sendingPermisson)
            //{
            //    Console.WriteLine("from if condition");
            //    //checkMissed(this, EventArgs.Empty);
            //    checkMissed(this,EventArgs.Empty);
            //    //return;
            //    //this.Invoke(new EventHandler(checkMissed)); //
            //}

        }
        public void forgridViewing(object sender , EventArgs e)
        {
            dataGrid.Rows.Add(RowAdder);

            //try
            //{
            //    for (int i = 0; i < 10; i++)
            //    {
            //        dataGrid.Rows[Row].Cells[i].Value = my_dict[i + 1][my_dict[i + 1].Count() - 1];
            //    }

            //    Row = Row + 1;
            //}
            //catch
            //{
            //    errorCatched = !errorCatched;
            //    RowAdder = 1;
            //}
            for (int i = 0; i < 10; i++)
            {
                try
                {
                    dataGrid.Rows[Row].Cells[i].Value = my_dict[i + 1][my_dict[i + 1].Count() - 1];
                }
                catch
                {
                    errorCatched = !errorCatched;
                    RowAdder = 1;
                }
            }
            Row = Row + 1;
            //catch
            //{
            //    errorCatched = !errorCatched;
            //    RowAdder = 1;
            //}
        }
        public void findGraph(object sender ,EventArgs e)
        {
            //graphDict[telemetry].Series[0].Points.DataBindY(my_dict[telemetry]);
            //graphDict[telemetry].DataBind();
            //Console.WriteLine($"Before Error Val is {currentTelemetry}");
            
            graphDict[currentTelemetry].Series[0].Points.DataBindY(my_dict[currentTelemetry]);
            graphDict[currentTelemetry].DataBind();
            
        }
        public void collectInDictGrph()
        {

            graphDict[2] = packGraph; // Package Number 
            graphDict[3] = presGraph; // Presure
            graphDict[4] = altGraph; // Altitude
            graphDict[5] = tempGraph; // temp Graph

        }
        private void startButton_Click(object sender, EventArgs e)
        {
            if (!clickedStartBefore)
            {
                //SerialPort1.PortName = portCombo.Text;
                //SerialPort1.BaudRate = 9600;
                //SerialPort1.BaudRate = 115200;
                //SerialPort1.Open();
                startButton.BackColor = Color.Green;
                clickedStartBefore = true;
                SerialPort1.Write("1-");
                MyStopWatchTimer.Reset();
                MyStopWatchTimer.Start();
                //connectionLabel.BackColor = Color.Green;
                //connectionLabel.Text = "Connected";
            }


        }

        private void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            data = SerialPort1.ReadLine(); // ["1 1","1 111","2 150"]
            //if (FirstCalculatingElapsed)
            //{
            //    tookTime = MyStopWatchTimer.Elapsed.TotalMilliseconds;
            //    FirstCalculatingElapsed = false;
            //    MyStopWatchTimer.Reset();
            //    MyStopWatchTimer.Stop();
            //    timer3.Interval = 1000-((int)tookTime);
            //    Console.WriteLine($"It took {tookTime} and Interval Changed to {1000-tookTime}");
            //}
            this.Invoke(new EventHandler(sendDataTextBox));
            //sendDataTextBox(this,EventArgs.Empty),
        }

        public void sendDataTextBox(object sender, EventArgs e)
        {
            // ["1 510, "2 333",]
            // "1 510"
            // 9 0-
            // 1 0-
            if (data.Contains(','))
            {
                
                string[] splitted_data = data.Split(',');
                if (splitted_data.Length != 10)
                    // 
                {
                    // Eksik Data olsa bile, yine de onları tut. Yeni data'gelmezse, eksik datalıyı kullanırsın..
                    Console.WriteLine("Lenght is not 10 So Missed Data Identified..");
                    missedDataAvailable = true;
                    //splitted_data.CopyTo(beforeJumped, 0);
                    beforeJumped = splitted_data;
                    if (!isTimerActivatedBefore)
                    {
                        sendingPermisson = true;
                        timer3.Stop(); // Comment 
                        timer3.Start();
                        isTimerActivatedBefore = true;
                    }
                    if (sendingPermisson)
                    {
                        // Serialport1.DiscardInbuffer falan arastir bi bakalım...
                        if (command_list.Count() > 0)
                        {
                            SerialPort1.Write($"1 {command_list.ElementAt(0).ToString()}-");
                            command_list.RemoveAt(0);
                        }
                        else
                        {
                            SerialPort1.Write("1 0-");
                        }
                    }

                }
                else
                {
                    errorCatched = false;
                    if (missedDataAvailable)
                    {
                        if (!isTimerActivatedBefore)
                        {
                            sendingPermisson = true;
                            timer3.Stop(); // Comment 
                            timer3.Start();
                            isTimerActivatedBefore = true;
                        }
                        Console.WriteLine("Missed Package Comed ...");
                        missedDataAvailable = false;
                        string[] packageNumbers = splitted_data[1].Split(' ');
                        if (!my_dict[Int32.Parse(packageNumbers[0])].Contains(Int32.Parse(packageNumbers[1])))
                        {
                            samePackage = false;
                            if (packageCounter != -1)
                            {
                                if (packageCounter+1 != Int32.Parse(packageNumbers[1]))
                                {
                                    // Ben burayı şu şekilde dusunerek yapmıstım , eksik datalı veriyi alıp beforejump'a
                                    // aktradılığdını düşünerek yaptım fakat eksik data hiç gelmemiş'te olabilr? o zaman burayı  da pass etmeli..
                                    // O if condition'u koşmalısın yani nasıl bunu anlarsın? packageCounter ve beforeJump' un içindeki package counter 'e göre
                                    // aynı zamanda şunu da yap belki beforeJump empty olacak o riski de göze al..
                                    Console.WriteLine("!!!Jumped DATA IDENTIFIED!!!");
                                    foreach (var jumped in beforeJumped)
                                    {
                                        string[] new_list = jumped.Split(' ');
                                        int TelVariable = Int32.Parse(new_list[0]);
                                        object sec = new_list[1];
                                        my_dict[TelVariable].Add(new_list[1]);
                                        if ((TelVariable == 2 || TelVariable == 3 || TelVariable == 4 || TelVariable == 5))
                                        {
                                            
                                            this.Invoke(new EventHandler(findGraph));
                                            //findGraph(first);
                                        }
                                        else if (TelVariable == 6)
                                        {
                                            statusLabel.Text = sec.ToString();
                                        }
                                    }
                                    this.Invoke(new EventHandler(forgridViewing));
                                }
                                packageCounter = Int32.Parse(packageNumbers[1]);
                            }
                            foreach (var variables in splitted_data)
                            {
                                // All variables to my_dict 
                                Console.WriteLine($"Variables is {variables}");
                                string[] new_list = variables.Split(' ');
                                int TelVariable = Int32.Parse(new_list[0]);
                                object sec = new_list[1];
                                my_dict[TelVariable].Add(new_list[1]);
                                currentTelemetry = TelVariable;
                                // findGraph kısmı belki serialport'a yazma işini yavaşlatabilir ekstra bir şey düşünebiliriz.
                                // Reached List gibi sonra Invoke ederiz o foreach eder orada sonra grafikleri doldurur...
                                if ((TelVariable == 2 || TelVariable == 3 || TelVariable == 4 || TelVariable == 5))
                                {

                                    this.Invoke(new EventHandler(findGraph));
                                    //findGraph(first);
                                }
                                else if (TelVariable == 6)
                                {
                                    statusLabel.Text = sec.ToString();
                                }
                            }
                            if (!missedDataAvailable && !samePackage)
                            {
                                this.Invoke(new EventHandler(forgridViewing));
                                //this.Invoke(new EventHandler(findGraph));
                            }
                        }
                        if (!samePackage && !missedDataAvailable && sendingVideoEnabled)
                        {
                            Console.WriteLine("All datas has been transferred and Sending Video Enabeld Writing now..");
                            if (sendingPermisson)
                            {
                                if (command_list.Count() == 0)
                                {
                                    if (!isSendedVideoSize)
                                    {
                                        SerialPort1.Write($"0 0 {totalVideoSize}-");
                                        //isSendedVideoSize = true;
                                    }
                                    else
                                    {
                                        SerialPort1.Write($"0 0 {videoBinary}-");
                                    }
                                    //SerialPort1.Write($"0 0 {videoBinary}-");
                                }
                                else
                                {
                                    if (!isSendedVideoSize)
                                    {
                                        SerialPort1.Write($"0 {command_list.ElementAt(0)} {totalVideoSize}-");
                                        //isSendedVideoSize = true;
                                    }
                                    else
                                    {
                                        SerialPort1.Write($"0 {command_list.ElementAt(0)} {videoBinary}-");
                                    }
                                    //SerialPort1.Write($"0 {command_list.ElementAt(0)} {videoBinary}-");
                                    command_list.RemoveAt(0);
                                }
                            }
                            
                            
                        }
                        else if (!samePackage && !missedDataAvailable)
                        {
                            Console.WriteLine("All datas has been transferred..");
                            if (sendingPermisson)
                            {
                                if (command_list.Count() == 0)
                                {
                                    SerialPort1.Write($"0 0-");
                                }
                                else
                                {
                                    SerialPort1.Write($"0 {command_list.ElementAt(0)}-");
                                    command_list.RemoveAt(0);
                                }
                            }
                            
                        }
                    }
                    else
                    {
                        Console.WriteLine($"hmm interesting problem... {data}");
                        missedDataAvailable = false;
                        samePackage = true; // Sonradan ekledik.
                        if (!isTimerActivatedBefore)
                        {
                            sendingPermisson = true;
                            timer3.Stop(); // Comment 
                            timer3.Start();
                            MyStopWatchTimer.Start();
                            
                            isTimerActivatedBefore = true;
                        }
                        string[] packageNumbers = splitted_data[1].Split(' ');
                        int pN = Int32.Parse(packageNumbers[1]);
                        if (pN == 1 || pN == 2)
                        {
                            if (pN == 1)
                            {
                                IntervalCalculator.Stop();
                                IntervalCalculator.Reset();
                                IntervalCalculator.Start();
                            }
                            else if (pN == 2)
                            {
                                tookTime = IntervalCalculator.Elapsed.TotalMilliseconds;
                                IntervalCalculator.Stop();
                                IntervalCalculator.Reset();
                                //timer3.Interval = (int)tookTime - 80;
                                Console.WriteLine($"It took {tookTime} and Interval Changed to {tookTime}");
                            }
                        }
                        if (!my_dict[Int32.Parse(packageNumbers[0])].Contains(Int32.Parse(packageNumbers[1])))
                        {
                            samePackage = false;
                            if (packageCounter != -1)
                            {
                                if (packageCounter + 1 != Int32.Parse(packageNumbers[1]))
                                {
                                    // Ben burayı şu şekilde dusunerek yapmıstım , eksik datalı veriyi alıp beforejump'a
                                    // aktradılığdını düşünerek yaptım fakat eksik data hiç gelmemiş'te olabilr? o zaman burayı  da pass etmeli..
                                    // O if condition'u koşmalısın yani nasıl bunu anlarsın? packageCounter ve beforeJump' un içindeki package counter 'e göre
                                    // aynı zamanda şunu da yap belki beforeJump empty olacak o riski de göze al..
                                    Console.WriteLine("!!!Jumped DATA IDENTIFIED!!!");
                                    foreach (var jumped in beforeJumped)
                                    {
                                        string[] new_list = jumped.Split(' ');
                                        int TelVariable = Int32.Parse(new_list[0]);
                                        object sec = new_list[1];
                                        my_dict[TelVariable].Add(new_list[1]);
                                        if ((TelVariable == 2 || TelVariable == 3 || TelVariable == 4 || TelVariable == 5))
                                        {

                                            this.Invoke(new EventHandler(findGraph));
                                            //findGraph(first);
                                        }
                                        else if (TelVariable == 6)
                                        {
                                            statusLabel.Text = sec.ToString();
                                        }
                                    }
                                    this.Invoke(new EventHandler(forgridViewing));
                                }
                                packageCounter = Int32.Parse(packageNumbers[1]);
                            }
                            foreach (var variables in splitted_data)
                            {
                                // All variables to my_dict 
                                Console.WriteLine($"Variables is {variables}");
                                string[] new_list = variables.Split(' ');
                                int TelVariable = Int32.Parse(new_list[0]);
                                object sec = new_list[1];
                                my_dict[TelVariable].Add(new_list[1]);
                                currentTelemetry = TelVariable;
                                // findGraph kısmı belki serialport'a yazma işini yavaşlatabilir ekstra bir şey düşünebiliriz.
                                // Reached List gibi sonra Invoke ederiz o foreach eder orada sonra grafikleri doldurur...
                                if ((TelVariable == 2 || TelVariable == 3 || TelVariable == 4 || TelVariable == 5))
                                {

                                    this.Invoke(new EventHandler(findGraph));
                                    //findGraph(first);
                                }
                                else if (TelVariable == 6)
                                {
                                    statusLabel.Text = sec.ToString();
                                }
                            }
                            if (!missedDataAvailable && !samePackage)
                            {
                                this.Invoke(new EventHandler(forgridViewing));
                                //this.Invoke(new EventHandler(findGraph));
                            }
                        }
                        if (!samePackage && !missedDataAvailable && sendingVideoEnabled)
                        {
                            Console.WriteLine("All datas has been transferred and Sending Video Enabeld Writing now..");
                            if (sendingPermisson)
                            {
                                if (command_list.Count() == 0)
                                {
                                    if (!isSendedVideoSize)
                                    {
                                        SerialPort1.Write($"0 0 {totalVideoSize}-");
                                        //isSendedVideoSize = true;
                                    }
                                    else
                                    {
                                        SerialPort1.Write($"0 0 {videoBinary}-");
                                    }
                                    //SerialPort1.Write($"0 0 {videoBinary}-");
                                }
                                else
                                {
                                    if (!isSendedVideoSize)
                                    {
                                        SerialPort1.Write($"0 {command_list.ElementAt(0)} {totalVideoSize}-");
                                        //isSendedVideoSize = true;
                                    }
                                    else
                                    {
                                        SerialPort1.Write($"0 {command_list.ElementAt(0)} {videoBinary}-");
                                    }
                                    //SerialPort1.Write($"0 {command_list.ElementAt(0)} {videoBinary}-");
                                    command_list.RemoveAt(0);
                                }
                            }
                        }
                        else if (!samePackage && !missedDataAvailable)
                        {
                            Console.WriteLine("All datas has been transferred..");
                            if (sendingPermisson)
                            {
                                if (command_list.Count() == 0)
                                {
                                    SerialPort1.Write($"0 0-");
                                }
                                else
                                {
                                    SerialPort1.Write($"0 {command_list.ElementAt(0)}-");
                                    command_list.RemoveAt(0);
                                }
                            }

                        }
                    }
                }
                
                
                //if (!missedDataAvailable && !samePackage)
                //{
                //    this.Invoke(new EventHandler(forgridViewing));
                //    //this.Invoke(new EventHandler(findGraph));
                //}

                //if (my_misseds.Count() == 0)
                //{
                //    missedDataAvailable = false;
                //}

                

                //if (!isTimerActivatedBefore)
                //{
                //    sendingPermisson = true;
                //    timer3.Stop(); // Comment 
                //    timer3.Start();
                //    isTimerActivatedBefore = true;

                //}

                //this.Invoke(new EventHandler(checkMissed)); //

                


            }

            else
            {
                Console.WriteLine($"Length NOT2-3 : {data}, {data.Length}");
                if (data.Length == 5) // 0 0 1
                {

                    Console.WriteLine("Video Binary Reached..->>>>");
                    //Thread.Sleep(20);
                    //transferredBytes += 24;
                    //transferredBytes += 40; // 0:40 , x:x+40
                    //transferredBytes += 237; // 0:40 , x:x+40
                    transferredBytes += 540; // 0:40 , x:x+40
                    byteTrans.Text = $"Transferred Bytes : {transferredBytes}";
                    if (sendingPermisson)
                    {
                        // CommandList if condition Ekle..
                        Console.WriteLine("Writing new binary...");
                        SerialPort1.Write($"0 0 {videoBinary}-");
                    }
                }
                else if (data.Length == 6)
                {
                    isVideoSizeReached = true;
                    isSendedVideoSize = true;
                    Console.WriteLine("------------------------Video Size Suessfully Reached---------------------");
                }
                else if (data.Length == 4)
                {
                    Console.WriteLine($"Data Time Interval is {data}"); // 955 -80
                    //timer3.Interval = Int32.Parse(data)-80;
                }
            }
        }
        public void checkMissed(object sender, EventArgs e)
        {

            IEnumerable<int> founded_misseds = getMissed(); // Eksik data Listesi..

            if (sendingPermisson)
            {
                if (my_misseds.Count() > 0)
                {
                    missedDataAvailable = true;
                    Console.WriteLine("Missed Data is Available....");
                    my_misseds.ForEach(Console.WriteLine);
                    Console.WriteLine("Writing now..");
                    if (sendingPermisson)
                    {
                        Console.WriteLine("Writed..");
                        //SerialPort1.Write(my_misseds.ElementAt(0).ToString());
                        string command_str = getCommandString();
                        //SerialPort1.Write(command_str);
                        string missed_str = $"1{command_str}";
                        if (command_list.Count() > 0)
                        {
                            command_list.RemoveAt(0);
                        }
                        currentlyMissed = my_misseds.ElementAt(0);
                        my_misseds.ForEach(temporary_missed.Add);
                        //temporary_missed = my_misseds;
                        Console.WriteLine($"Writed string is :{missed_str}");
                        //SerialPort1.Write(missed_str); // Normally this is activated..
                        char[] serialTest;
                        serialTest = missed_str.ToCharArray();
                        SerialPort1.Write(serialTest, 0, serialTest.Length);
                        if (!isTimerOnlineBefore)
                        {
                            timer5.Start();
                            isTimerOnlineBefore = true;
                        }
                        else
                        {
                            timer5.Stop();
                            timer5.Start();
                        }
                        
                        //if (!isTimerOnlineBefore)
                        //{
                        //    Console.WriteLine("Timer is started...");
                        //    timer5.Start();
                        //    isTimerOnlineBefore = true;
                        //}
                        //else
                        //{
                        //    Console.WriteLine("Timer is stoppedEnabled started...");
                        //    timer5.Stop();
                        //    timer5.Start();
                        //}
                    }
                    else
                    {
                        Console.WriteLine("Because of the permisson NOT!");
                    }
                }
                else
                {
                    currentlyMissed = 0;
                    Console.WriteLine("NO MISSED AVALABLE!!!!!");
                    missedDataAvailable = false;
                    missedisDone = true;
                    if (sendingPermisson)
                    {
                        missedDataAvailable = false;
                        //SerialPort1.Write("0",0,1);
                        string will_concenate = "0";
                        string command_str = getCommandString();
                        will_concenate += command_str;
                        char[] serialTest;
                        serialTest = will_concenate.ToCharArray();
                        //SerialPort1.Write(serialTest, 0, serialTest.Length);
                        //SerialPort1.Write(command_str);
                        Console.WriteLine($"Writed string is :0{command_str}");
                        SerialPort1.Write($"0{command_str}");
                        if (command_list.Count() > 0)
                        {
                            command_list.RemoveAt(0);
                        }
                        //SerialPort1.Write($"0 {command_str}");
                        timer5.Start();
                        //if (sendingVideoEnabled)
                        //{
                        //    //timer5.Start();
                        //    if (!isTimerOnlineBefore)
                        //    {
                        //        timer5.Start();
                        //        isTimerOnlineBefore = true;
                        //    }
                        //    else
                        //    {
                        //        timer5.Stop();
                        //        timer5.Start();
                        //    }


                        //}
                    }
                    else
                    {
                        Console.WriteLine("ELSEE--Because of the permisson NOT!");
                        
                    }
                }
            }
            else
            {
                missedDataAvailable = false;
                timer5.Stop();
                return;
            }

        }


        public string getCommandString()
        {
            string command_str;
            if (command_list.Count() > 0)
            {
                if (sendingVideoEnabled)
                {
                    command_str = $" {command_list[0].ToString()} {videoBinary}-";
                }
                else
                {
                    command_str = $" {command_list[0].ToString()}-";
                }
                //command_list.RemoveAt(0);
            }
            else
            {
                if (sendingVideoEnabled)
                {
                    command_str = $" 0 {videoBinary}-";
                }
                else
                {
                    command_str = " 0-";
                }
                
            }
            return command_str;
        }
        //public string getCommandString()
        //{
        //    string command_str;
        //    if (command_list.Count() > 0)
        //    {
        //        command_str = $"{command_list[0].ToString()}-";
        //        command_list.RemoveAt(0);
        //    }
        //    else
        //    {
        //        command_str = "0-";
        //    }
        //    return command_str;
        //}
        public IEnumerable<int> getMissed()
        {
            // Eksik dataları fonuded ediyor...
            Console.WriteLine("Reached list printing..");
            reached_list.ForEach(Console.WriteLine);
            IEnumerable<int> founded_missed = list_range.Except(reached_list);
            my_misseds = founded_missed.ToList();
            return founded_missed;
        }

        private void releaseBut_Click(object sender, EventArgs e)
        {
            sendReleaseCommand = true;
            command_list.Add(88);
            releaseBut.BackColor = Color.Green;
        }

        private void activeAlBut_Click(object sender, EventArgs e)
        {
            motorActiveAlways = true;
            command_list.Add(99);
            activeAlBut.BackColor = Color.Green;
            // Color ekle.
        }

        private void testtingWhile(object sender, EventArgs e)
        {
            Console.WriteLine("ITS ONLINE ");
        }
        private void canSendTimer(object sender, EventArgs e)
        {
            Console.WriteLine("Sending Permission is blocked now..");
            Console.WriteLine($"Gecen Zaman : {MyStopWatchTimer.Elapsed.TotalMilliseconds}");
            sendingPermisson = false;
            timer3.Stop();
            MyStopWatchTimer.Stop();
            MyStopWatchTimer.Reset();
            reached_list.Clear();
            isTimerActivatedBefore = false;
        }

        private void active10SecBut_Click(object sender, EventArgs e)
        {
            motorActive10sec = true;
            command_list.Add(77);
            active10SecBut.BackColor = Color.Yellow;
            timer1.Start();

        }
        public void receiveTimer(object sender, EventArgs e)
        {
            timer5.Stop();
            //isTimedOut = true;
            //isTimerOnlineBefore = false;
            ////Console.WriteLine("-------------------------100ms Timer Timed Out Trying to send one more time..-----------");
            //if (my_misseds.Contains(currentlyMissed) && !reached_list.Contains(currentlyMissed))
            //{
            //    Console.WriteLine("-------------------------100ms Timer Timed Out Trying to send one more time..-----------");
            //    checkMissed();
            //}

        }
        public void timer10Seconds(object sender, EventArgs e)
        {
            timer1.Stop();
            active10SecBut.BackColor = default(Color);
        }

        public void timerforColor(object sender, EventArgs e)
        {
            if (changedColorStatus)
            {
                statusLabel.ForeColor = default(Color);
            }
            else
            {
                statusLabel.ForeColor = Color.Green;

            }
            changedColorStatus = !changedColorStatus;
        }

        private void send_video_Click(object sender, EventArgs e)
        {
            sendingVideoEnabled = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SerialPort1.PortName = portCombo.Text;
            //SerialPort1.BaudRate = 9600;
            SerialPort1.BaudRate = 115200;
            SerialPort1.ReadBufferSize = SerialPort1.ReadBufferSize + 4096;
            SerialPort1.WriteBufferSize = SerialPort1.WriteBufferSize + 4096;
            SerialPort1.Open();
            
            connectionLabel.BackColor = Color.Green;
            connectionLabel.Text = "Connected";
        }

    }
}
