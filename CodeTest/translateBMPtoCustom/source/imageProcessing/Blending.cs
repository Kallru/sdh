using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Media.Media3D;

namespace imageProcessing
{
    public static class Blending
    {
        public static Vector3D Divide( Vector3D vector, Vector3D intput )
        {
            return new Vector3D( vector.X / intput.X, vector.Y / intput.Y, vector.Z / intput.Z );
        }

        public static Vector3D ConvertTo( Color color )
        {
            return new Vector3D( ( double )color.R / 255.0, ( double )color.G / 255.0, ( double )color.B / 255.0 );
        }

        public static Color ConvertToColor( Vector3D color, bool bScalePixel = true )
        {
            if( bScalePixel )
                color *= 255.0;

            return Color.FromArgb( 255, ( int )color.X, ( int )color.Y, ( int )color.Z );
        }

        // 알파블렌딩
        // R = ( source * alpha ) + ( destination * ( 1 - alpha ) )
        public static Color Alpha( Color sourceColor, Color destColor, float alpha )
        {
            Vector3D source = ConvertTo( sourceColor );
            Vector3D dest = ConvertTo( destColor );

            var final = ( ( source * alpha ) + ( dest * ( 1.0f - alpha ) ) );
            return ConvertToColor( final );
        }
        
        // 리니어 닷지 
        // R = Base + Blend
        public static Color LinearDodge( Color sourceColor, Color destColor )
        {
            Vector3D source = ConvertTo( sourceColor );
            Vector3D dest = ConvertTo( destColor );

            var r = source + dest;
            return ConvertToColor( new Vector3D( Math.Min( r.X, 1.0 ), Math.Min( r.Y, 1.0 ), Math.Min( r.Z, 1.0 ) ) );
        }

        // 컬러 닷지
        // R = Base / (1-Blend)
        public static Color ColorDodge( Color sourceColor, Color destColor )
        {
            Vector3D source = ConvertTo( sourceColor );
            Vector3D dest = ConvertTo( destColor );

            var r = Divide( source, Vector3D.Subtract( new Vector3D( 1.0, 1.0, 1.0 ), dest ) );
            return ConvertToColor( new Vector3D( Math.Min( r.X, 1.0 ), Math.Min( r.Y, 1.0 ), Math.Min( r.Z, 1.0 ) ) );
        }
    }
}
