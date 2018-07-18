#include "Surface.h"
#include "ChiliWin.h"
#include <cassert>
#include <fstream>
#include "Graphics.h"

Surface::Surface( const std::string& filename )
{
	std::ifstream file( filename,std::ios::binary );
	assert( file );

	BITMAPFILEHEADER bmFileHeader;
	file.read( reinterpret_cast< char* >( &bmFileHeader ),
		sizeof( bmFileHeader ) );

	BITMAPINFOHEADER bmInfoHeader;
	file.read( reinterpret_cast< char* >( &bmInfoHeader ),
		sizeof( bmInfoHeader ) );

	assert( bmInfoHeader.biBitCount == 24 ||
		bmInfoHeader.biBitCount == 32 );
	assert( bmInfoHeader.biCompression == BI_RGB );

	const bool is32b = bmInfoHeader.biBitCount == 32;

	width = bmInfoHeader.biWidth;

	// Test for reverse row order and
	//  control y loop accordingly.
	int yStart;
	int yEnd;
	int dy;
	if( bmInfoHeader.biHeight < 0 )
	{
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmInfoHeader.biHeight;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}

	pixels.resize( width * height );

	file.seekg( bmFileHeader.bfOffBits );
	// Padding is for 24 bit depth only.
	const int padding = ( 4 - ( width * 3 ) % 4 ) % 4;

	for( int y = yStart; y != yEnd; y += dy )
	{
		for( int x = 0; x < width; ++x )
		{
			PutPixel( x,y,Color( file.get(),file.get(),file.get() ) );
			if( is32b )
			{
				file.seekg( 1,std::ios::cur );
			}
		}
		if( !is32b )
		{
			file.seekg( padding,std::ios::cur );
		}
	}
}

Surface::Surface( const std::wstring& fileName )
{
	std::string properString( fileName.begin(),fileName.end() );
	properString.assign( fileName.begin(),fileName.end() );
	*this = Surface( properString );
}

Surface::Surface( const Surface& other,const RectI& clip )
	:
	width( clip.GetWidth() ),
	height( clip.GetHeight() )
{
	pixels.resize( width * height );

	int i = 0;
	int j = 0;
	for( int y = clip.top; y < clip.bottom - 1; ++y )
	{
		for( int x = clip.left; x < clip.right - 1; ++x )
		{
			PutPixel( j,i,other.GetPixel( x,y ) );
			++j;
		}
		j = 0;
		++i;
	}
}

Surface::Surface( int width,int height )
	:
	width( width ),
	height( height ),
	pixels( width * height )
{
}

Surface::Surface( Surface&& donor )
{
	*this = std::move( donor );
}

Surface& Surface::operator=( Surface&& rhs )
{
	width = rhs.width;
	height = rhs.height;
	pixels = std::move( rhs.pixels );

	rhs.width = 0;
	rhs.height = 0;

	return( *this );
}

void Surface::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < width );
	assert( y >= 0 );
	assert( y < height );
	pixels.data()[y * width + x] = c;
}

void Surface::DrawRect( int x,int y,int width,int height,Color c )
{
	for( int i = y; i < y + height; ++i )
	{
		for( int j = x; j < x + width; ++j )
		{
			PutPixel( j,i,c );
		}
	}
}

Color Surface::GetPixel( int x,int y ) const
{
	assert( x >= 0 );
	assert( x < width );
	assert( y >= 0 );
	assert( y < height );
	return pixels.data()[y * width + x];
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

RectI Surface::GetRect() const
{
	return{ 0,width,0,height };
}

Surface Surface::GetExpanded( int width,int height ) const
{
	Surface bigger = { width,height };
	const Vec2 expandAmount = { float( width / this->width ),
		float( height / this->height ) };

	for( int y = 0; y < this->height; ++y )
	{
		for( int x = 0; x < this->width; ++x )
		{
			bigger.DrawRect( int( float( x ) *
				expandAmount.x ),int( float( y ) *
				expandAmount.y ),
				int( expandAmount.x ),
				int( expandAmount.y ),
				GetPixel( x,y ) );
		}
	}

	return bigger;
}

// https://rosettacode.org/wiki/Bilinear_interpolation helped a lot with this conversion code.
Surface Surface::GetInterpolated( int width,int height ) const
{
	const int newWidth = width;
	const int newHeight = height;
	Surface newImage = Surface( newWidth,newHeight );
	for( int x = 0; x < newWidth; ++x )
	{
		for( int y = 0; y < newHeight; ++y )
		{
			const float gx = ( float( x ) ) / newWidth * ( GetWidth() - 1 );
			const float gy = ( float( y ) ) / newHeight * ( GetHeight() - 1 );
			const int gxi = int( gx );
			const int gyi = int( gy );
			int rgb = 0;
			const int c00 = GetPixel( gxi,gyi ).dword;
			const int c10 = GetPixel( gxi + 1,gyi ).dword;
			const int c01 = GetPixel( gxi,gyi + 1 ).dword;
			const int c11 = GetPixel( gxi + 1,gyi + 1 ).dword;
			for( int i = 0; i <= 2; ++i )
			{
				const int b00 = ( c00 >> ( i * 8 ) ) & 0xFF;
				const int b10 = ( c10 >> ( i * 8 ) ) & 0xFF;
				const int b01 = ( c01 >> ( i * 8 ) ) & 0xFF;
				const int b11 = ( c11 >> ( i * 8 ) ) & 0xFF;
				const int ble = ( int( Vec2
					::Blerp( float( b00 ),float( b10 ),
					float( b01 ),float( b11 ),
					gx - float( gxi ),gy - float( gyi ) ) ) )
					<< ( 8 * i );
				rgb = rgb | ble;
			}
			newImage.PutPixel( x,y,rgb );
		}
	}
	return newImage;
}
