using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace packing
{
    public partial class Form1 : Form
    {
        private List<FileBuffer> m_files = new List<FileBuffer>();

        public struct FileBuffer
        {
            public string Name;
            public int FileSize;
            public byte[] buffer;
        }

        public Form1()
        {
            InitializeComponent();
        }

        // "열기" 이벤트
        private void button3_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "*.pack|*.pack";
            openFileDialog1.FileName = "";
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                m_files.Clear();

                using (FileStream fs = new FileStream(openFileDialog1.FileName, FileMode.Open))
                {
                    using (BinaryReader br = new BinaryReader(fs))
                    {
                        int totalFileCount = br.ReadInt32();
                        for( int i = 0 ; i < totalFileCount ; ++i )
                        {
                            FileBuffer file = new FileBuffer();
                            int nameLength = br.ReadInt32();
                            file.Name = br.ReadString();
                            file.FileSize = br.ReadInt32();
                            file.buffer = br.ReadBytes(file.FileSize);

                            m_files.Add(file);
                        }
                    }
                }

                listBox1.Items.Clear();
                foreach (FileBuffer file in m_files)
                {
                    listBox1.Items.Add(file.Name);
                }
            }
        }

        // "추가" 버튼 이벤트
        private void button1_Click(object sender, EventArgs e)
        {
            if( openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK )
            {
                listBox1.Items.AddRange(openFileDialog1.FileNames);
            }
        }

        // "저장" 이벤트
        private void button2_Click(object sender, EventArgs e)
        {
            if( listBox1.Items.Count == 0 )
            {
                MessageBox.Show("The List is empty");
                return;
            }

            if (saveFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                using (FileStream fs = new FileStream(saveFileDialog1.FileName, FileMode.Create))
                {
                    using (BinaryWriter binaryWriter = new BinaryWriter(fs))
                    {
                        // 포함된 전체 파일 개수
                        binaryWriter.Write((int)listBox1.Items.Count);

                        foreach (string fileFullName in listBox1.Items)
                        {
                            FileInfo info = new FileInfo(fileFullName);
                            if( info.Exists )
                            {
                                // 파일 이름 길이
                                binaryWriter.Write((int)info.Name.Length);
                                // 파일 이름
                                binaryWriter.Write(info.Name);

                                // Write binary data
                                using( FileStream target = info.OpenRead() )
                                {
                                    // 파일 사이즈
                                    binaryWriter.Write((int)target.Length);

                                    byte[] buffer = new byte[target.Length];
                                    target.Read(buffer, 0, (int)target.Length);
                                    binaryWriter.Write(buffer);
                                }
                            }
                            else
                            {
                                // error
                            }
                        }
                    }
                }

                MessageBox.Show("완료");
            }
        }

        // "풀기" 이벤트
        private void button4_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                foreach (FileBuffer file in m_files)
                {
                    string fullPath = Path.Combine(folderBrowserDialog1.SelectedPath, file.Name);

                    using (FileStream fs = new FileStream(fullPath, FileMode.Create))
                    {
                        using (BinaryWriter bw = new BinaryWriter(fs))
                        {
                            bw.Write(file.buffer, 0, file.FileSize);
                        }
                    }
                }

                MessageBox.Show("완료");
            }
        }
    }
}
