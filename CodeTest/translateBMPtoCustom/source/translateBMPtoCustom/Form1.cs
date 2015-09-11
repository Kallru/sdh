using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using imageProcessing;
using System.Windows;

namespace translateBMPtoCustom
{
    public partial class Form1 : Form
    {
        private Bitmap m_sourceImage;
        public Form1()
        {
            InitializeComponent();
        }

        private void ConvertBMPtoCustom( string sourceFilePath )
        {
            MemoryStream memoryBuffer = new MemoryStream();

            // Bitmap에서 컬러값만 추출
            Bitmap sourceImage = new Bitmap( Image.FromFile( sourceFilePath ) );
            {
                BitmapData data = sourceImage.LockBits( new Rectangle( 0, 0, sourceImage.Width, sourceImage.Height ), ImageLockMode.ReadOnly, sourceImage.PixelFormat );

                int dataSize = data.Stride * data.Height;
                byte[] rawData = new byte[dataSize];
                Marshal.Copy( data.Scan0, rawData, 0, dataSize );

                memoryBuffer.Write( rawData, 0, dataSize );

                sourceImage.UnlockBits( data );
            }

            memoryBuffer.Seek( 0, SeekOrigin.Begin );
            
            // Save
            string saveFilePath = Path.Combine( Path.GetDirectoryName( sourceFilePath ), Path.GetFileNameWithoutExtension( sourceFilePath ) + ".hiy" );
            using( FileStream fs = new FileStream( saveFilePath, FileMode.Create ) )
            {
                using( BinaryWriter binaryWriter = new BinaryWriter( fs ) )
                {
                    // Header
                    binaryWriter.Write( "Hi" );
                    binaryWriter.Write( sourceImage.Width );
                    binaryWriter.Write( sourceImage.Height );

                    for( int y = 0 ; y < sourceImage.Height ; ++y )
                    {
                        for( int x = 0 ; x < sourceImage.Width ; ++x )
                        {
                            int b = memoryBuffer.ReadByte();
                            int g = memoryBuffer.ReadByte();
                            int r = memoryBuffer.ReadByte();
                            int a = memoryBuffer.ReadByte();

                            // 저장용, 저장은 RGB로 저장
                            binaryWriter.Write( r );
                            binaryWriter.Write( g );
                            binaryWriter.Write( b );
                        }
                    }
                }
            }

            memoryBuffer.Close();

            MessageBox.Show( "Convert Done", "OK" );
        }

        private void LoadMyFormat( string fileName )
        {
            using( FileStream fs = new FileStream( fileName, FileMode.Open ) )
            {
                using( BinaryReader binaryReader = new BinaryReader( fs ) )
                {
                    byte[] nameTag = binaryReader.ReadBytes( 3 );
                    int width = binaryReader.ReadInt32();
                    int height = binaryReader.ReadInt32();

                    // 나머지 데이터
                    Bitmap image = new Bitmap( width, height );
                    for( int y = 0 ; y < height ; ++y )
                    {
                        for( int x = 0 ; x < width ; ++x )
                        {
                            int r = binaryReader.ReadInt32();
                            int g = binaryReader.ReadInt32();
                            int b = binaryReader.ReadInt32();

                            image.SetPixel( x, y, Color.FromArgb( r, g, b) );
                        }
                    }

                    m_sourceImage = image;
                    pictureBox1.Image = image;

                    // pictureBox1.Image.Save( @"d:\Work\CodeTest\sdh\CodeTest\translateBMPtoCustom\data\test.bmp" );
                }
            }
        }

        private void button1_Click( object sender, EventArgs e )
        {
            if( openFileDialog1.ShowDialog() == DialogResult.OK )
            {
                ConvertBMPtoCustom( openFileDialog1.FileName );
            }
        }

        private void button2_Click( object sender, EventArgs e )
        {
            if( openFileDialog1.ShowDialog() == DialogResult.OK )
            {
                LoadMyFormat( openFileDialog1.FileName );
            }
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            Bitmap bitmapImage = (Bitmap)m_sourceImage.Clone();

            var rawData = bitmapImage.LockBits( new Rectangle( 0, 0, bitmapImage.Width, bitmapImage.Height ), ImageLockMode.ReadWrite, bitmapImage.PixelFormat );

            float alphaF = ((float)trackBar1.Value) / 255.0f;
            int alpha = (int)(alphaF * 255.0f);

            unsafe
            {
                int* pBuffer = (int*)rawData.Scan0.ToPointer();

                for (int y = 0; y < rawData.Height; ++y)
                {
                    for (int x = 0; x < rawData.Width; ++x)
                    {
                        int pitch = (rawData.Width * y) + x;

                        Color sourceColor = Color.FromArgb(pBuffer[pitch]);

                        // 요건 알파채널 사용 버전
                        //pBuffer[pitch] = Color.FromArgb(alpha, newColor.R, newColor.G, newColor.B).ToArgb();

                        pBuffer[pitch] = Blending.Alpha( sourceColor, Color.Red, alphaF ).ToArgb();
                    }
                }
            }

            bitmapImage.UnlockBits(rawData);

            pictureBox1.Image = bitmapImage;
            pictureBox1.Refresh();
        }

        private void TwoImageBlendingTemplate( Func<Color,Color,Color> blendMode )
        {
            Bitmap sourceImage = null;
            Bitmap destImage = null;
            if( openFileDialog1.ShowDialog() != DialogResult.OK )
            {
                return ;
            }
            sourceImage = new Bitmap( openFileDialog1.FileName );

            if( openFileDialog1.ShowDialog() != DialogResult.OK )
            {
                return ;
            }
            destImage = new Bitmap( openFileDialog1.FileName );

            var dest = destImage.LockBits( new Rectangle( 0, 0, destImage.Width, destImage.Height ), ImageLockMode.ReadOnly, destImage.PixelFormat );
            var source = sourceImage.LockBits( new Rectangle( 0, 0, sourceImage.Width, sourceImage.Height ), ImageLockMode.ReadWrite, sourceImage.PixelFormat );

            unsafe
            {
                int* pDestBuffer = ( int* )dest.Scan0.ToPointer();
                int* pSourceBuffer = ( int* )source.Scan0.ToPointer();

                for( int y = 0 ; y < source.Height ; ++y )
                {
                    for( int x = 0 ; x < source.Width ; ++x )
                    {
                        int pitch = ( source.Width * y ) + x;

                        Color sourceColor = Color.FromArgb( pSourceBuffer[pitch] );
                        Color destColor = Color.FromArgb( pDestBuffer[pitch] );

                        pSourceBuffer[pitch] = blendMode( sourceColor, destColor ).ToArgb();
                    }
                }
            }

            sourceImage.UnlockBits( source );
            destImage.UnlockBits( dest );

            pictureBox1.Image = sourceImage;
            pictureBox1.Refresh();
        }

        private void button3_Click( object sender, EventArgs e )
        {
            TwoImageBlendingTemplate( ( source, dest ) => Blending.LinearDodge( source, dest ) );
        }

        private void button4_Click( object sender, EventArgs e )
        {
            TwoImageBlendingTemplate( ( source, dest ) => Blending.ColorDodge( source, dest ) );
        }
    }
}
