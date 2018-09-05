/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */
	#include "acwe.h"

	#include <stdio.h>
	//#include "platform.h"
	#include "xil_printf.h"


	/***************************** Include Files *********************************/

	#include "xparameters.h"	/* SDK generated parameters */
	#include "xsdps.h"		/* SD device driver */
	#include "xil_printf.h"
	#include "ff.h"
	#include "xil_cache.h"
	#include "xplatform_info.h"
#include "LenaOnCode.h"


/***************************** Include Files *********************************/

#include "xparameters.h"	/* SDK generated parameters */
#include "xsdps.h"		/* SD device driver */
#include "xil_printf.h"
#include "ff.h"
#include "xil_cache.h"
#include "xplatform_info.h"
#include "xdoimgproc.h"
#include "xaxidma.h"
#include "axitimerhelper.h"

#define SIZE_ARR (320*240)
// 3x3 kernel
#define KERNEL_DIM 3

// Memory used by DMA
#define MEM_BASE_ADDR 	0x01000000
#define TX_BUFFER_BASE	(MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE	(MEM_BASE_ADDR + 0x00300000)
// Get a pointer to the TX and RX dma buffer (CONFIGURE DMA)
// The pointers are for 8-bit memory but their addresses are 32 bit (u32)
unsigned char *m_dma_buffer_TX = (unsigned char*)TX_BUFFER_BASE;
unsigned char *m_dma_buffer_RX = (unsigned char*)RX_BUFFER_BASE;

unsigned char imgIn_HW[SIZE_ARR];


XAxiDma axiDma;
int initDMA()
{
	XAxiDma_Config *CfgPtr;
	CfgPtr = XAxiDma_LookupConfig(XPAR_AXI_DMA_0_DEVICE_ID);
	XAxiDma_CfgInitialize(&axiDma,CfgPtr);

	// Disable interrupts
	XAxiDma_IntrDisable(&axiDma,XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&axiDma,XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);

	return XST_SUCCESS;
}

XDoimgproc doImgProc;
int initDoImgProc()
{
	int status;

	XDoimgproc_Config *doImgProc_cfg;
	doImgProc_cfg = XDoimgproc_LookupConfig(XPAR_DOIMGPROC_0_DEVICE_ID);
	if (!doImgProc_cfg)
	{
		printf("Error loading config for doHist_cfg\n");
	}
	status = XDoimgproc_CfgInitialize(&doImgProc,doImgProc_cfg);
	if (status != XST_SUCCESS)
	{
		printf("Error initializing for doHist\n");
	}

	return status;
}

// Impulse
/*char kernel[KERNEL_DIM*KERNEL_DIM] = {
		0, 0, 0,
		0, 1, 0,
		0, 0, 0,
};*/

// Edge
/*char kernel[KERNEL_DIM*KERNEL_DIM] = {
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1,
};*/

// Use with morphological (Erode, Dilate)
char kernel[KERNEL_DIM*KERNEL_DIM] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
};


/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int InitFfsSd(void);

/************************** Variable Definitions *****************************/
//static FIL fil;		/* File object */
static FATFS fatfs;


int main ()
{

	int Status;
	AxiTimerHelper axiTimer;


	  TImage Image;
	  double HW_elapsed;

	   	int x1;

	    int iteracoes,smoothing;
	    char szFileName[200];

		xil_printf("SD Polled File System Example Test \r\n");
		axiTimer.startTimer();

		Status = InitFfsSd();
		axiTimer.stopTimer();

		HW_elapsed = axiTimer.getElapsedTimerInSeconds();
		printf("HW execution time: %f sec\n", HW_elapsed);

		if (Status != XST_SUCCESS) {
			xil_printf("SD Polled File System Example Test failed \r\n");
			return XST_FAILURE;
		}

		xil_printf("Successfully ran SD Polled File System Example Test \r\n");

		x1=1 ;
		iteracoes=0;
		smoothing=0;
		strcpy(szFileName,"full");

		acwex(x1,szFileName, &Image, iteracoes,smoothing);


		strcat(szFileName,"ou");

		 // save the segmented image do the disk
			savemhdraw(szFileName,&Image);

		        // free the memory used for storing the segmented image
			freeimage(&Image);

		//return XST_SUCCESS;

			initDMA();
			initDoImgProc();

			printf("Doing convolution on HW\n");

			// Populate data (Get image from header and put on memory)
			for (int idx = 0; idx < SIZE_ARR; idx++)
			{
				imgIn_HW[idx] = img[idx];
			}


			printf("Starting.... HW\n");
			// Ask for a convolution
			XDoimgproc_Write_kernel_Bytes(&doImgProc,0,kernel,9);
			printf("Kernel total bytes: %d Bitwidth:%d Base: 0x%X\n",XDoimgproc_Get_kernel_TotalBytes(&doImgProc), XDoimgproc_Get_kernel_BitWidth(&doImgProc), XDoimgproc_Get_kernel_BaseAddress(&doImgProc));
			XDoimgproc_Set_operation(&doImgProc,2);
			XDoimgproc_Start(&doImgProc);

			// Do the DMA transfer to push and get our image
			axiTimer.startTimer();
			Xil_DCacheFlushRange((u32)imgIn_HW,SIZE_ARR*sizeof(unsigned char));
			Xil_DCacheFlushRange((u32)m_dma_buffer_RX,SIZE_ARR*sizeof(unsigned char));

			XAxiDma_SimpleTransfer(&axiDma,(u32)imgIn_HW,SIZE_ARR*sizeof(unsigned char),XAXIDMA_DMA_TO_DEVICE);
			XAxiDma_SimpleTransfer(&axiDma,(u32)m_dma_buffer_RX,SIZE_ARR*sizeof(unsigned char),XAXIDMA_DEVICE_TO_DMA);

			//Wait transfers to finish
			while(XAxiDma_Busy(&axiDma,XAXIDMA_DMA_TO_DEVICE));
			while(XAxiDma_Busy(&axiDma,XAXIDMA_DEVICE_TO_DMA));

			// Invalidate the cache to avoid reading garbage
			Xil_DCacheInvalidateRange((u32)m_dma_buffer_RX,SIZE_ARR*sizeof(unsigned char));
			axiTimer.stopTimer();

			HW_elapsed = axiTimer.getElapsedTimerInSeconds();
			printf("HW execution time: %f sec\n", HW_elapsed);


			return 0;
}

/*****************************************************************************/
/**
*
* @file xilffs_polled_example.c
*
*
* @note This example uses file system with SD to write to and read from
* an SD card using ADMA2 in polled mode.
* To test this example File System should not be in Read Only mode.
* To test this example USE_MKFS option should be true.
*
* This example was tested using SD2.0 card and eMMC (using eMMC to SD adaptor).
*
* To test with different logical drives, drive number should be mentioned in
* both FileName and Path variables. By default, it will take drive 0 if drive
* number is not mentioned in the FileName variable.
* For example, to test logical drive 1
* FileName =  "1:/<file_name>" and Path = "1:/"
* Similarly to test logical drive N, FileName = "N:/<file_name>" and
* Path = "N:/"
*
* None.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who Date     Changes
* ----- --- -------- -----------------------------------------------
* 1.00a hk  10/17/13 First release
* 2.2   hk  07/28/14 Make changes to enable use of data cache.
* 2.5   sk  07/15/15 Used File size as 8KB to test on emulation platform.
* 2.9   sk  06/09/16 Added support for mkfs.
*
*</pre>
*
******************************************************************************/

/*****************************************************************************/
/**
*
* Main function to call the SD example.
*
* @param	None
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/


/*****************************************************************************/
/**
*
* File system example using SD driver to write to and read from an SD card
* in polled mode. This example creates a new file on an
* SD card (which is previously formatted with FATFS), write data to the file
* and reads the same data back to verify.
*
* @param	None
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int InitFfsSd(void)
{
	FRESULT Res;
	UINT NumBytesRead;
	UINT NumBytesWritten;
	u32 BuffCnt;
	u32 FileSize = (80*1024*1024);
	/*
	 * To test logical drive 0, Path should be "0:/"
	 * For logical drive 1, Path should be "1:/"
	 */
	TCHAR *Path = "0:/";

	/*Platform = XGetPlatform_Info();
	if (Platform == XPLAT_ZYNQ_ULTRA_MP) {

		FileSize = 8*1024;
	}

	for(BuffCnt = 0; BuffCnt < FileSize; BuffCnt++){
		SourceAddress[BuffCnt] = TEST + BuffCnt;
	}*/

	/*
	 * Register volume work area, initialize device
	 */
	Res = f_mount(&fatfs, Path, 0);

	if (Res != FR_OK) {
		return XST_FAILURE;
	}

	/*
	 * Path - Path to logical driver, 0 - FDISK format.
	 * 0 - Cluster size is automatically determined based on Vol size.
	 */
	/*Res = f_mkfs(Path, 0, 0);
	if (Res != FR_OK) {
		return XST_FAILURE;
	}*/

	/*
	 * Open file with required permissions.
	 * Here - Creating new file with read/write permissions. .
	 * To open file with write permissions, file system should not
	 * be in Read Only mode.
	 */
	//SD_File = (char *)FileName;
	//Res = f_open(&fil, SD_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);

/*	Res = f_open(&fil, SD_File, FA_READ );
	if (Res) {
		return XST_FAILURE;
	}*/

	/*
	 * Pointer to beginning of file .
	 */
/*	Res = f_lseek(&fil, 0);
	if (Res) {
		return XST_FAILURE;
	}*/

	/*
	 * Write data to file.
	 */
	/*Res = f_write(&fil, (const void*)SourceAddress, FileSize,
			&NumBytesWritten);
	if (Res) {
		return XST_FAILURE;
	}*/

	/*
	 * Pointer to beginning of file .
	 */
	/*Res = f_lseek(&fil, 0);
	if (Res) {
		return XST_FAILURE;
	}*/

	/*
	 * Read data from file.
	 */
	/*Res = f_read(&fil, (void*)DestinationAddress, FileSize,
			&NumBytesRead);
	if (Res) {
		return XST_FAILURE;
	}*/

	/*
	 * Data verification
	 */
	/*for(BuffCnt = 0; BuffCnt < FileSize; BuffCnt++){
		if(SourceAddress[BuffCnt] != DestinationAddress[BuffCnt]){
			return XST_FAILURE;
		}
	}*/

	/*
	 * Close file.
	 */
	/*Res = f_close(&fil);
	if (Res) {
		return XST_FAILURE;
	}*/

	return XST_SUCCESS;
}

