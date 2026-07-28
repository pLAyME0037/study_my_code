namespace WinFormsApp1
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            textBoxSex = new TextBox();
            textBoxName = new TextBox();
            button1 = new Button();
            sex = new Label();
            name = new Label();
            buttonDefaulltForm = new Button();
            SuspendLayout();
            // 
            // textBoxSex
            // 
            textBoxSex.Location = new Point(367, 184);
            textBoxSex.Name = "textBoxSex";
            textBoxSex.Size = new Size(142, 27);
            textBoxSex.TabIndex = 9;
            // 
            // textBoxName
            // 
            textBoxName.Location = new Point(367, 151);
            textBoxName.Name = "textBoxName";
            textBoxName.Size = new Size(142, 27);
            textBoxName.TabIndex = 8;
            // 
            // button1
            // 
            button1.Location = new Point(312, 245);
            button1.Name = "button1";
            button1.Size = new Size(115, 54);
            button1.TabIndex = 7;
            button1.Text = "Add";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // sex
            // 
            sex.AutoSize = true;
            sex.Location = new Point(309, 184);
            sex.Name = "sex";
            sex.Size = new Size(32, 20);
            sex.TabIndex = 6;
            sex.Text = "Sex";
            // 
            // name
            // 
            name.AutoSize = true;
            name.Location = new Point(292, 151);
            name.Name = "name";
            name.Size = new Size(49, 20);
            name.TabIndex = 5;
            name.Text = "Name";
            // 
            // buttonDefaulltForm
            // 
            buttonDefaulltForm.Location = new Point(458, 248);
            buttonDefaulltForm.Name = "buttonDefaulltForm";
            buttonDefaulltForm.Size = new Size(104, 49);
            buttonDefaulltForm.TabIndex = 10;
            buttonDefaulltForm.Text = "Default Form";
            buttonDefaulltForm.UseVisualStyleBackColor = true;
            buttonDefaulltForm.Click += buttonDefaulltForm_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(buttonDefaulltForm);
            Controls.Add(textBoxSex);
            Controls.Add(textBoxName);
            Controls.Add(button1);
            Controls.Add(sex);
            Controls.Add(name);
            Name = "Form1";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Form1";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox textBoxSex;
        private TextBox textBoxName;
        private Button button1;
        private Label sex;
        private Label name;
        private Button buttonDefaulltForm;
    }
}
