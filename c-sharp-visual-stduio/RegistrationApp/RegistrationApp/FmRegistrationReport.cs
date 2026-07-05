using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.Reporting.WinForms;
using System.Drawing.Printing;

using RegistrationApp.Models;

namespace RegistrationApp
{
    public partial class FmRegistrationReport : Form
    {
        //Attribute
        public int RegistrationId { get; set; }
        public FmRegistrationReport()
        {
            InitializeComponent();
        }

        private void FmRegistrationReport_Load(object sender, EventArgs e)
        {
            RegistrationModel Md = new RegistrationModel();
            RegistrationModel NewMd = new RegistrationModel();
            NewMd = Md.RegistrationInfo(RegistrationId);

            //Set parameters
            ReportParameter[] parameters = new ReportParameter[5];
            parameters[0] = new ReportParameter("RegistrationId", NewMd.RegistrationId.ToString());
            parameters[1] = new ReportParameter("StudentCode", NewMd.StudentCode.ToString());
            parameters[2] = new ReportParameter("Name", NewMd.Name.ToString());
            parameters[3] = new ReportParameter("Sex", NewMd.Sex.ToString());
            parameters[4] = new ReportParameter("DoB", NewMd.DoB.ToString("dd/MMM/yyyy"));

            reportViewer1.LocalReport.SetParameters(parameters);

            reportViewer1.RefreshReport();
        }
    }
}
