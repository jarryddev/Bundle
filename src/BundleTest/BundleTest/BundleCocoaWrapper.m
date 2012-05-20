/*

Copyright (c) <2012>, <Jarryd Hall, Taher Odeh>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.

*/

// Bundle_CocoaWrapper.m

#import "BundleCocoaWrapper.h"

@interface Bundle_CocoaWrapper ()

@end

@implementation Bundle_CocoaWrapper

- (void) bundleStart:(NSString *)filename withMData:(struct mappedData *)mData
{
    NSString *bundleRoot = [[NSBundle mainBundle] bundlePath];
    NSString *filePath = [bundleRoot stringByAppendingPathComponent:filename];
    bundle_start([filePath fileSystemRepresentation], mData);
}

- (NSData *) bundle_useFile:(NSString *) fileName withMappedData:(struct mappedData *)mData andPointer:(NSData *) data
{
	// convert NSString to char *
	const char *file = [fileName sys];
	
	// get index data for file
	offset_p offset = bundle_getIndexDataFor(file, (long) mData->mappedAddress);
	
    // create temp pointer
    void *offsetPointer = &offset->offset_start;
    
	// create data from file in mapped segment

	data = [NSData dataWithBytesNoCopy:offsetPointer
                                length:offset->size 
                        freeWhenDone:NO];
	
	return data;
}

// returns 1 for compressed, 0 for uncompressed
- (NSNumber *) isFileCompressed:(NSString *) fileName withMappedData:(struct mappedData *)mData
{
	// convert NSString to char *
	const char *file = [fileName fileSystemRepresentation];
	
	// get index data for file
	offset_p offset = bundle_getIndexDataFor(file, (long)mData->mappedAddress);
	
	return [NSNumber numberWithChar:offset->compressed];
}



@end