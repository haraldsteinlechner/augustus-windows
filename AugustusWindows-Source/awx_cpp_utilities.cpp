///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   © Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////

#include "awx_cpp_utilities.h"


// Spiegelt alle ‚àö¬∫bergebenen Bytes von pvData. Funktion aus gamedev.com (User: RedAnt@gmx.de). Also keine AWX Funktion
bool reverseByteOrder( void* pvData, unsigned int nUnitScale, unsigned int nUnitsInArray )
{
    if ( pvData == 0x00 || nUnitsInArray < 1  || nUnitScale < 1 )
    {
        return false;
    }

    // Reversing the byte order makes little sense for byte sequences.
    if ( nUnitScale == 1 )
    {
        return true;
    }

    unsigned char* pData = reinterpret_cast < unsigned char* > ( pvData );

    // If our unit scale is 4 bytes, we need to do 2 byte swaps.
    unsigned int nByteSwaps = nUnitScale / 2;
    unsigned char aux;

    for ( unsigned int iUnit = 0; iUnit < nUnitsInArray; ++iUnit )
    {
        /* Swap bytes:
           0 <==> 3
           1 <==> 2
           (Values descriptive for a 4-bytes-per-unit scenario)
        */
        for ( unsigned int iByte = 0; iByte < nByteSwaps; ++iByte )
        {
            // Store front byte in aux register.
            aux = pData[ iByte ];

            // Overwrite old front byte with value of the byte at the back.
            pData[ iByte ] = pData[ nUnitScale - ( iByte + 1 ) ];

            // Overwrite old back byte with the value stored in aux.
            pData[ nUnitScale - ( iByte + 1 ) ] = aux;
        }

        // Go to next unit.
        pData += nUnitScale;
    }

    return true;
}

AWXCPUType getCPUArchitecture()
{
	// Über lib CPU abfragen
	return AWXx86;
	//return AWXppc;
}


// Funktion wie fread nur wird auf WindowsTargets die ByteFolge gespiegelt
// um das BigEndian des PowerPCs in LittleEndian zu konvertieren

int awx_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	int result = (int)fread(ptr,size, nmemb, stream);
	if(getCPUArchitecture()==AWXx86)
		reverseByteOrder(ptr, (unsigned int)size, (unsigned int)nmemb );
	
	return result;
}

int awx_freadT(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	int result = (int)fread(ptr,size, nmemb, stream);
	reverseByteOrder(ptr, (unsigned int)size, (unsigned int)nmemb );
	return result;
}


int awx_fread(void *ptr, size_t size, size_t nmemb, FILE *stream , bool inverted)
{
	int result = (int)fread(ptr,size, nmemb, stream);
	if(inverted)reverseByteOrder(ptr, (unsigned int)size, (unsigned int)nmemb );
	
	
	return result;
}

int awx_fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	void* temptr = malloc(size*nmemb);
	memcpy(temptr,ptr,size*nmemb);
	
	#ifndef __ppc
		reverseByteOrder(temptr, (unsigned int)size, (unsigned int)nmemb );
	#endif
	
	int result = (int)fwrite(temptr,size, nmemb, stream);

	free(temptr);
	
	return result;
} 