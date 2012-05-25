//
//  ViewController.m
//  iOSTest
//
//  Created by Jarryd Hall on 5/23/12.
//  Copyright (c) 2012 Jarryd Hall. All rights reserved.
//

#import "ViewController.h"
#import "BundleCocoaWrapper.h"
#import "integration.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize image;

BundleCocoaWrapper *wrapper;
struct mappedData *mData;

- (void) startBundle
{
    // test bundle here.
    
    // create wrapper object to use methods
    wrapper = [[BundleCocoaWrapper alloc] init];
    
    // create struct pointer. (holds mapped address and filesize)
    mData = malloc(sizeof(struct mappedData));
    
    [wrapper bundleStart:@"testImages.pak" withMData:mData];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	[self startBundle];
}

- (void)viewDidUnload
{
    [self setImage:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (void) writeToFileTest:(NSData *)dataToWrite
{
    NSString *docsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *path = [docsDirectory stringByAppendingPathComponent:@"fileName.txt"];
    
    // Write the file
    [dataToWrite writeToFile:path atomically:YES];
    
    // Read the file
    UIImage *imageFromFile = [[UIImage alloc] initWithContentsOfFile:path];
    //self.image = [[UIImageView alloc] init];
    image.image = imageFromFile;
    
    // Check if file exists
    NSFileManager *fileManager = [NSFileManager defaultManager];
    [fileManager fileExistsAtPath:path]; // Returns a BOOL    

}

- (IBAction)get1:(id)sender {
    NSData *data = NULL;
    //data = [wrapper bundle_useFile:@"1.jpg" withMappedData:mData andPointer:data];
    
    NSString *file = @"1.jpg";
    
    offset_p offset = bundle_getIndexDataFor([file fileSystemRepresentation], mData->mappedAddress);
    if (offset != nil) {
        data = [NSData dataWithBytesNoCopy:mData->mappedAddress+310 length:628406 freeWhenDone:NO];
//        data = [NSData dataWithBytesNoCopy:mData->mappedAddress+310 length:628406 freeWhenDone:NO];
    }
    
    if (data == nil) {
        NSLog(@".........");
    }
    else
    {
    //    [self writeToFileTest:data];
        image.image = [UIImage imageWithData:data];
    }
}

- (IBAction)get6:(id)sender {
    
}
- (void)dealloc {
    [image release];
    [super dealloc];
}
@end
