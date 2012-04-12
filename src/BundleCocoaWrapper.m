// Bundle_CocoaWrapper.m
//

#import integration.c

// this will 
id bundle_useFile:(NSString *) fileName forObject:(id) object
{
	/* convert string to char *
	 * pass the char * to void bundle_getIndexDataFor(char *fileName)
	 * use the index data to pass to 
	 * object = [NSData dataWithBytesNoCopy: pointerToMMappedData 
	 *                  						length: dataLength  
	 *                      					freeWhenDone: NO];
	 * 
	 * example usage: UIImage* anImage = [UIImage imageWithData: object];
	 * will decide to add the return or what to complete this method with.
	 * return object
	 */
}