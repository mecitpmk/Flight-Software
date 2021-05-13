
namespace WinFormSerialing
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
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.Connect_Button = new System.Windows.Forms.Button();
            this.PortLists = new System.Windows.Forms.ComboBox();
            this.Manual_Motor_Always = new System.Windows.Forms.Button();
            this.TenSecond_Active = new System.Windows.Forms.Button();
            this.Manual_Release = new System.Windows.Forms.Button();
            this.SEND_VIDEO = new System.Windows.Forms.Button();
            this.PermissionTimer = new System.Windows.Forms.Timer(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.TOTAL_PACK = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.TRANSFER_STATUS = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.DataRECV_EVENT);
            // 
            // Connect_Button
            // 
            this.Connect_Button.Location = new System.Drawing.Point(516, 49);
            this.Connect_Button.Name = "Connect_Button";
            this.Connect_Button.Size = new System.Drawing.Size(196, 47);
            this.Connect_Button.TabIndex = 0;
            this.Connect_Button.Text = "Connect";
            this.Connect_Button.UseVisualStyleBackColor = true;
            this.Connect_Button.Click += new System.EventHandler(this.Connect_Event);
            // 
            // PortLists
            // 
            this.PortLists.FormattingEnabled = true;
            this.PortLists.Location = new System.Drawing.Point(370, 63);
            this.PortLists.Name = "PortLists";
            this.PortLists.Size = new System.Drawing.Size(121, 21);
            this.PortLists.TabIndex = 1;
            // 
            // Manual_Motor_Always
            // 
            this.Manual_Motor_Always.ForeColor = System.Drawing.Color.CornflowerBlue;
            this.Manual_Motor_Always.Location = new System.Drawing.Point(50, 135);
            this.Manual_Motor_Always.Name = "Manual_Motor_Always";
            this.Manual_Motor_Always.Size = new System.Drawing.Size(196, 47);
            this.Manual_Motor_Always.TabIndex = 2;
            this.Manual_Motor_Always.Text = "Manual Motor Always";
            this.Manual_Motor_Always.UseVisualStyleBackColor = true;
            this.Manual_Motor_Always.Click += new System.EventHandler(this.Manual_Motor_Always_Click);
            // 
            // TenSecond_Active
            // 
            this.TenSecond_Active.ForeColor = System.Drawing.Color.CornflowerBlue;
            this.TenSecond_Active.Location = new System.Drawing.Point(265, 135);
            this.TenSecond_Active.Name = "TenSecond_Active";
            this.TenSecond_Active.Size = new System.Drawing.Size(196, 47);
            this.TenSecond_Active.TabIndex = 3;
            this.TenSecond_Active.Text = "Motor Active 10 Sec";
            this.TenSecond_Active.UseVisualStyleBackColor = true;
            this.TenSecond_Active.Click += new System.EventHandler(this.TenSecond_Active_Click);
            // 
            // Manual_Release
            // 
            this.Manual_Release.ForeColor = System.Drawing.Color.CornflowerBlue;
            this.Manual_Release.Location = new System.Drawing.Point(485, 135);
            this.Manual_Release.Name = "Manual_Release";
            this.Manual_Release.Size = new System.Drawing.Size(196, 47);
            this.Manual_Release.TabIndex = 4;
            this.Manual_Release.Text = "Manual Release";
            this.Manual_Release.UseVisualStyleBackColor = true;
            this.Manual_Release.Click += new System.EventHandler(this.Manual_Release_Click);
            // 
            // SEND_VIDEO
            // 
            this.SEND_VIDEO.ForeColor = System.Drawing.Color.CornflowerBlue;
            this.SEND_VIDEO.Location = new System.Drawing.Point(706, 135);
            this.SEND_VIDEO.Name = "SEND_VIDEO";
            this.SEND_VIDEO.Size = new System.Drawing.Size(196, 47);
            this.SEND_VIDEO.TabIndex = 5;
            this.SEND_VIDEO.Text = "Send Video";
            this.SEND_VIDEO.UseVisualStyleBackColor = true;
            this.SEND_VIDEO.Click += new System.EventHandler(this.SEND_VIDEO_Click);
            // 
            // PermissionTimer
            // 
            this.PermissionTimer.Tick += new System.EventHandler(this.Permission_OVER_EVENT);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(47, 218);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Package:";
            // 
            // TOTAL_PACK
            // 
            this.TOTAL_PACK.AutoSize = true;
            this.TOTAL_PACK.Location = new System.Drawing.Point(106, 218);
            this.TOTAL_PACK.Name = "TOTAL_PACK";
            this.TOTAL_PACK.Size = new System.Drawing.Size(0, 13);
            this.TOTAL_PACK.TabIndex = 7;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(47, 250);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(156, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "VIDEO TRANSFER STATUS : ";
            // 
            // TRANSFER_STATUS
            // 
            this.TRANSFER_STATUS.AutoSize = true;
            this.TRANSFER_STATUS.Location = new System.Drawing.Point(209, 250);
            this.TRANSFER_STATUS.Name = "TRANSFER_STATUS";
            this.TRANSFER_STATUS.Size = new System.Drawing.Size(0, 13);
            this.TRANSFER_STATUS.TabIndex = 9;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(998, 519);
            this.Controls.Add(this.TRANSFER_STATUS);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.TOTAL_PACK);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.SEND_VIDEO);
            this.Controls.Add(this.Manual_Release);
            this.Controls.Add(this.TenSecond_Active);
            this.Controls.Add(this.Manual_Motor_Always);
            this.Controls.Add(this.PortLists);
            this.Controls.Add(this.Connect_Button);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button Connect_Button;
        private System.Windows.Forms.ComboBox PortLists;
        private System.Windows.Forms.Button Manual_Motor_Always;
        private System.Windows.Forms.Button TenSecond_Active;
        private System.Windows.Forms.Button Manual_Release;
        private System.Windows.Forms.Button SEND_VIDEO;
        private System.Windows.Forms.Timer PermissionTimer;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label TOTAL_PACK;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label TRANSFER_STATUS;
    }
}

