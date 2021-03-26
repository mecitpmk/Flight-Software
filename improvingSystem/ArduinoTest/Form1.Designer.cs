namespace ArduinoTest
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea4 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend4 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series4 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            this.startButton = new System.Windows.Forms.Button();
            this.portCombo = new System.Windows.Forms.ComboBox();
            this.SerialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.releaseBut = new System.Windows.Forms.Button();
            this.activeAlBut = new System.Windows.Forms.Button();
            this.active10SecBut = new System.Windows.Forms.Button();
            this.packGraph = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.presGraph = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.tempGraph = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.altGraph = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.statusLabel = new System.Windows.Forms.Label();
            this.connectionLabel = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.label2 = new System.Windows.Forms.Label();
            this.dataGrid = new System.Windows.Forms.DataGridView();
            this.TEAM_ID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.PACKAGE_NUM = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.PRESSURE = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ALTITUDE = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.TEMPERATURE = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.STATUS = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.PITCH = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ROLL = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.YAW = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.TURN_NUM = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.send_video = new System.Windows.Forms.Button();
            this.timer3 = new System.Windows.Forms.Timer(this.components);
            this.timer4 = new System.Windows.Forms.Timer(this.components);
            this.timer5 = new System.Windows.Forms.Timer(this.components);
            this.button1 = new System.Windows.Forms.Button();
            this.byteTrans = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.packGraph)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.presGraph)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tempGraph)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.altGraph)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGrid)).BeginInit();
            this.SuspendLayout();
            // 
            // startButton
            // 
            this.startButton.Location = new System.Drawing.Point(151, 137);
            this.startButton.Name = "startButton";
            this.startButton.Size = new System.Drawing.Size(168, 51);
            this.startButton.TabIndex = 0;
            this.startButton.Text = "Start to Take";
            this.startButton.UseVisualStyleBackColor = true;
            this.startButton.Click += new System.EventHandler(this.startButton_Click);
            // 
            // portCombo
            // 
            this.portCombo.FormattingEnabled = true;
            this.portCombo.Location = new System.Drawing.Point(542, 89);
            this.portCombo.Name = "portCombo";
            this.portCombo.Size = new System.Drawing.Size(145, 21);
            this.portCombo.TabIndex = 1;
            // 
            // SerialPort1
            // 
            this.SerialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SerialPort1_DataReceived);
            // 
            // releaseBut
            // 
            this.releaseBut.Location = new System.Drawing.Point(355, 137);
            this.releaseBut.Name = "releaseBut";
            this.releaseBut.Size = new System.Drawing.Size(168, 51);
            this.releaseBut.TabIndex = 2;
            this.releaseBut.Text = "Send Release Command";
            this.releaseBut.UseVisualStyleBackColor = true;
            this.releaseBut.Click += new System.EventHandler(this.releaseBut_Click);
            // 
            // activeAlBut
            // 
            this.activeAlBut.Location = new System.Drawing.Point(572, 137);
            this.activeAlBut.Name = "activeAlBut";
            this.activeAlBut.Size = new System.Drawing.Size(168, 51);
            this.activeAlBut.TabIndex = 3;
            this.activeAlBut.Text = "Active Always";
            this.activeAlBut.UseVisualStyleBackColor = true;
            this.activeAlBut.Click += new System.EventHandler(this.activeAlBut_Click);
            // 
            // active10SecBut
            // 
            this.active10SecBut.Location = new System.Drawing.Point(789, 137);
            this.active10SecBut.Name = "active10SecBut";
            this.active10SecBut.Size = new System.Drawing.Size(168, 51);
            this.active10SecBut.TabIndex = 4;
            this.active10SecBut.Text = "Active for 10 Sec";
            this.active10SecBut.UseVisualStyleBackColor = true;
            this.active10SecBut.Click += new System.EventHandler(this.active10SecBut_Click);
            // 
            // packGraph
            // 
            chartArea1.AxisX.Title = "Time(t)";
            chartArea1.AxisY.Title = "Package Number";
            chartArea1.Name = "ChartArea1";
            this.packGraph.ChartAreas.Add(chartArea1);
            legend1.Alignment = System.Drawing.StringAlignment.Center;
            legend1.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Top;
            legend1.Name = "Legend1";
            this.packGraph.Legends.Add(legend1);
            this.packGraph.Location = new System.Drawing.Point(59, 260);
            this.packGraph.Name = "packGraph";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series1.Legend = "Legend1";
            series1.Name = "Package Graph";
            this.packGraph.Series.Add(series1);
            this.packGraph.Size = new System.Drawing.Size(260, 260);
            this.packGraph.TabIndex = 5;
            this.packGraph.Text = "packGraph";
            // 
            // presGraph
            // 
            chartArea2.AxisX.Title = "Time(t)";
            chartArea2.AxisY.Title = "Pascal (Pa)";
            chartArea2.Name = "ChartArea1";
            this.presGraph.ChartAreas.Add(chartArea2);
            legend2.Alignment = System.Drawing.StringAlignment.Center;
            legend2.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Top;
            legend2.Name = "Legend1";
            this.presGraph.Legends.Add(legend2);
            this.presGraph.Location = new System.Drawing.Point(676, 260);
            this.presGraph.Name = "presGraph";
            series2.BackImageAlignment = System.Windows.Forms.DataVisualization.Charting.ChartImageAlignmentStyle.Center;
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series2.Color = System.Drawing.Color.Fuchsia;
            series2.Legend = "Legend1";
            series2.Name = "Pres Graph";
            this.presGraph.Series.Add(series2);
            this.presGraph.Size = new System.Drawing.Size(260, 260);
            this.presGraph.TabIndex = 7;
            this.presGraph.Text = "presGraph";
            // 
            // tempGraph
            // 
            chartArea3.AxisX.Title = "Time (t)";
            chartArea3.AxisY.Title = "Celcius (C)";
            chartArea3.Name = "ChartArea1";
            this.tempGraph.ChartAreas.Add(chartArea3);
            legend3.Alignment = System.Drawing.StringAlignment.Center;
            legend3.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Top;
            legend3.Name = "Legend1";
            this.tempGraph.Legends.Add(legend3);
            this.tempGraph.Location = new System.Drawing.Point(992, 260);
            this.tempGraph.Name = "tempGraph";
            series3.BackImageAlignment = System.Windows.Forms.DataVisualization.Charting.ChartImageAlignmentStyle.Center;
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series3.Color = System.Drawing.Color.Orange;
            series3.Legend = "Legend1";
            series3.Name = "Temp Graph";
            this.tempGraph.Series.Add(series3);
            this.tempGraph.Size = new System.Drawing.Size(260, 260);
            this.tempGraph.TabIndex = 8;
            this.tempGraph.Text = "tempGraph";
            // 
            // altGraph
            // 
            chartArea4.AxisX.Title = "Time(t)";
            chartArea4.AxisY.Title = "Meeters (m)";
            chartArea4.Name = "ChartArea1";
            this.altGraph.ChartAreas.Add(chartArea4);
            legend4.Alignment = System.Drawing.StringAlignment.Center;
            legend4.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Top;
            legend4.Name = "Legend1";
            this.altGraph.Legends.Add(legend4);
            this.altGraph.Location = new System.Drawing.Point(370, 260);
            this.altGraph.Name = "altGraph";
            series4.BackImageAlignment = System.Windows.Forms.DataVisualization.Charting.ChartImageAlignmentStyle.Center;
            series4.ChartArea = "ChartArea1";
            series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series4.Color = System.Drawing.Color.Red;
            series4.Legend = "Legend1";
            series4.Name = "Altitude Graph";
            this.altGraph.Series.Add(series4);
            this.altGraph.Size = new System.Drawing.Size(260, 260);
            this.altGraph.TabIndex = 9;
            this.altGraph.Text = "altGraph";
            // 
            // timer1
            // 
            this.timer1.Interval = 10900;
            this.timer1.Tick += new System.EventHandler(this.timer10Seconds);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.label1.Location = new System.Drawing.Point(1024, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(130, 20);
            this.label1.TabIndex = 11;
            this.label1.Text = "Satelleite Status:";
            // 
            // statusLabel
            // 
            this.statusLabel.AutoSize = true;
            this.statusLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.statusLabel.Location = new System.Drawing.Point(1160, 18);
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(79, 20);
            this.statusLabel.TabIndex = 12;
            this.statusLabel.Text = "Waiting..";
            this.statusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // connectionLabel
            // 
            this.connectionLabel.AutoSize = true;
            this.connectionLabel.BackColor = System.Drawing.Color.Red;
            this.connectionLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.connectionLabel.Location = new System.Drawing.Point(163, 18);
            this.connectionLabel.Name = "connectionLabel";
            this.connectionLabel.Size = new System.Drawing.Size(129, 20);
            this.connectionLabel.TabIndex = 13;
            this.connectionLabel.Text = "Not Connected";
            this.connectionLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.label4.Location = new System.Drawing.Point(12, 18);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(145, 20);
            this.label4.TabIndex = 14;
            this.label4.Text = "Connection Status:";
            // 
            // timer2
            // 
            this.timer2.Interval = 500;
            this.timer2.Tick += new System.EventHandler(this.timerforColor);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.label2.Location = new System.Drawing.Point(412, 91);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(125, 18);
            this.label2.TabIndex = 15;
            this.label2.Text = "Available Ports:";
            // 
            // dataGrid
            // 
            this.dataGrid.AllowUserToAddRows = false;
            this.dataGrid.AllowUserToDeleteRows = false;
            this.dataGrid.AllowUserToResizeColumns = false;
            this.dataGrid.AllowUserToResizeRows = false;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.dataGrid.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGrid.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.dataGrid.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle2.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGrid.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
            this.dataGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.TEAM_ID,
            this.PACKAGE_NUM,
            this.PRESSURE,
            this.ALTITUDE,
            this.TEMPERATURE,
            this.STATUS,
            this.PITCH,
            this.ROLL,
            this.YAW,
            this.TURN_NUM});
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle3.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            dataGridViewCellStyle3.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGrid.DefaultCellStyle = dataGridViewCellStyle3;
            this.dataGrid.Location = new System.Drawing.Point(59, 540);
            this.dataGrid.Name = "dataGrid";
            this.dataGrid.ReadOnly = true;
            dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle4.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            dataGridViewCellStyle4.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle4.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle4.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle4.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGrid.RowHeadersDefaultCellStyle = dataGridViewCellStyle4;
            dataGridViewCellStyle5.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.dataGrid.RowsDefaultCellStyle = dataGridViewCellStyle5;
            this.dataGrid.Size = new System.Drawing.Size(1193, 178);
            this.dataGrid.TabIndex = 16;
            // 
            // TEAM_ID
            // 
            this.TEAM_ID.HeaderText = "TEAM ID";
            this.TEAM_ID.Name = "TEAM_ID";
            this.TEAM_ID.ReadOnly = true;
            // 
            // PACKAGE_NUM
            // 
            this.PACKAGE_NUM.HeaderText = "PACKAGE NUM";
            this.PACKAGE_NUM.Name = "PACKAGE_NUM";
            this.PACKAGE_NUM.ReadOnly = true;
            // 
            // PRESSURE
            // 
            this.PRESSURE.HeaderText = "PRESSURE";
            this.PRESSURE.Name = "PRESSURE";
            this.PRESSURE.ReadOnly = true;
            // 
            // ALTITUDE
            // 
            this.ALTITUDE.HeaderText = "ALTITUDE";
            this.ALTITUDE.Name = "ALTITUDE";
            this.ALTITUDE.ReadOnly = true;
            // 
            // TEMPERATURE
            // 
            this.TEMPERATURE.HeaderText = "TEMPERATURE";
            this.TEMPERATURE.Name = "TEMPERATURE";
            this.TEMPERATURE.ReadOnly = true;
            // 
            // STATUS
            // 
            this.STATUS.HeaderText = "STATUS";
            this.STATUS.Name = "STATUS";
            this.STATUS.ReadOnly = true;
            // 
            // PITCH
            // 
            this.PITCH.HeaderText = "PITCH";
            this.PITCH.Name = "PITCH";
            this.PITCH.ReadOnly = true;
            // 
            // ROLL
            // 
            this.ROLL.HeaderText = "ROLL";
            this.ROLL.Name = "ROLL";
            this.ROLL.ReadOnly = true;
            // 
            // YAW
            // 
            this.YAW.HeaderText = "YAW";
            this.YAW.Name = "YAW";
            this.YAW.ReadOnly = true;
            // 
            // TURN_NUM
            // 
            this.TURN_NUM.HeaderText = "TURN NUM";
            this.TURN_NUM.Name = "TURN_NUM";
            this.TURN_NUM.ReadOnly = true;
            // 
            // send_video
            // 
            this.send_video.Location = new System.Drawing.Point(1006, 137);
            this.send_video.Name = "send_video";
            this.send_video.Size = new System.Drawing.Size(168, 51);
            this.send_video.TabIndex = 18;
            this.send_video.Text = "Send Vid";
            this.send_video.UseVisualStyleBackColor = true;
            this.send_video.Click += new System.EventHandler(this.send_video_Click);
            // 
            // timer3
            // 
            this.timer3.Interval = 490;
            this.timer3.Tick += new System.EventHandler(this.canSendTimer);
            // 
            // timer4
            // 
            this.timer4.Interval = 500;
            this.timer4.Tick += new System.EventHandler(this.testtingWhile);
            // 
            // timer5
            // 
            this.timer5.Tick += new System.EventHandler(this.receiveTimer);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(709, 87);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(107, 23);
            this.button1.TabIndex = 19;
            this.button1.Text = "Connect";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // byteTrans
            // 
            this.byteTrans.AutoSize = true;
            this.byteTrans.Location = new System.Drawing.Point(1048, 203);
            this.byteTrans.Name = "byteTrans";
            this.byteTrans.Size = new System.Drawing.Size(94, 13);
            this.byteTrans.TabIndex = 20;
            this.byteTrans.Text = "0 Byte Transferred";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1329, 742);
            this.Controls.Add(this.byteTrans);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.send_video);
            this.Controls.Add(this.dataGrid);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.connectionLabel);
            this.Controls.Add(this.statusLabel);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.altGraph);
            this.Controls.Add(this.tempGraph);
            this.Controls.Add(this.presGraph);
            this.Controls.Add(this.packGraph);
            this.Controls.Add(this.active10SecBut);
            this.Controls.Add(this.activeAlBut);
            this.Controls.Add(this.releaseBut);
            this.Controls.Add(this.portCombo);
            this.Controls.Add(this.startButton);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.packGraph)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.presGraph)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tempGraph)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.altGraph)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGrid)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button startButton;
        private System.Windows.Forms.ComboBox portCombo;
        public System.IO.Ports.SerialPort SerialPort1;
        private System.Windows.Forms.Button releaseBut;
        private System.Windows.Forms.Button activeAlBut;
        private System.Windows.Forms.Button active10SecBut;
        public System.Windows.Forms.DataVisualization.Charting.Chart packGraph;
        public System.Windows.Forms.DataVisualization.Charting.Chart presGraph;
        public System.Windows.Forms.DataVisualization.Charting.Chart tempGraph;
        public System.Windows.Forms.DataVisualization.Charting.Chart altGraph;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label statusLabel;
        private System.Windows.Forms.Label connectionLabel;
        private System.Windows.Forms.Label label4;
        public System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.Label label2;
        public System.Windows.Forms.DataGridView dataGrid;
        private System.Windows.Forms.DataGridViewTextBoxColumn TEAM_ID;
        private System.Windows.Forms.DataGridViewTextBoxColumn PACKAGE_NUM;
        private System.Windows.Forms.DataGridViewTextBoxColumn PRESSURE;
        private System.Windows.Forms.DataGridViewTextBoxColumn ALTITUDE;
        private System.Windows.Forms.DataGridViewTextBoxColumn TEMPERATURE;
        private System.Windows.Forms.DataGridViewTextBoxColumn STATUS;
        private System.Windows.Forms.DataGridViewTextBoxColumn PITCH;
        private System.Windows.Forms.DataGridViewTextBoxColumn ROLL;
        private System.Windows.Forms.DataGridViewTextBoxColumn YAW;
        private System.Windows.Forms.DataGridViewTextBoxColumn TURN_NUM;
        private System.Windows.Forms.Button send_video;
        private System.Windows.Forms.Timer timer3;
        private System.Windows.Forms.Timer timer4;
        private System.Windows.Forms.Timer timer5;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label byteTrans;
    }
}

