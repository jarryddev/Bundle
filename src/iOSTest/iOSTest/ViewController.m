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

- (IBAction)get1:(id)sender {
    NSData *data = NULL;
    data = [wrapper bundle_useFile:@"1.jpg" withMappedData:mData andPointer:data];
    NSString *someString = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
    NSLog(@"Data is %@", someString);
    image.image = [UIImage imageWithData:data];
}

- (IBAction)get6:(id)sender {
    
}
- (void)dealloc {
    [image release];
    [super dealloc];
}
@end
