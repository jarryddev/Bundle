//
//  ViewController.m
//  BundleTest
//
//  Created by Jarryd Hall on 5/17/12.
//  Copyright (c) 2012 Jarryd Hall. All rights reserved.
//

#import "ViewController.h"
#import "BundleCocoaWrapper.h"


@interface ViewController ()

@end

@implementation ViewController

@synthesize imageView;
@synthesize textField;

struct mappedData *mData;
Bundle_CocoaWrapper *wrapper;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSString *bundleRoot = [[NSBundle mainBundle] bundlePath];
    NSFileManager *manager = [NSFileManager defaultManager];
    NSDirectoryEnumerator *direnum = [manager enumeratorAtPath:bundleRoot];
    
    NSString *filename;
    
    NSLog(@"Bunde Root: %@", bundleRoot);
    while ((filename = [direnum nextObject] )) {
        
        
        if ([filename hasSuffix:@".pak"]) {   //change the suffix to what you are looking for
             
            NSLog(@"%@", filename);
        }
        
    }


    
    // setup bundle
    mData = malloc(sizeof(struct mappedData));
    wrapper = [[Bundle_CocoaWrapper alloc] init];
}

- (IBAction)getFile:(id)sender
{
    [textField resignFirstResponder];
    NSLog(@"Text field .text is %@", textField.text);
    printf("mData in viewDidLoad is %X", mData->mappedAddress);
    NSData *data = [wrapper bundle_useFile:textField.text withMappedData:mData];
    imageView.image = [UIImage imageWithData:data];
}

- (IBAction)loadTwenty:(id)sender
{
    NSString *filename = @"testImages.pak";
    bundle_start([filename fileSystemRepresentation], mData);
}

- (IBAction)loadForty:(id)sender
{
    NSString *filename = @"test40.pak";
    bundle_start([filename fileSystemRepresentation], mData);
    NSLog(@"......");
}

- (IBAction)loadEighty:(id)sender
{
    NSString *filename = @"test80.pak";
    bundle_start([filename fileSystemRepresentation], mData);
}

- (IBAction)loadOneSixty:(id)sender
{
    NSString *filename = @"test160.pak";
    bundle_start([filename fileSystemRepresentation], mData);
}

- (IBAction)twentyMbSeq:(id)sender
{
    NSString *temp;
    NSData *data;
    for (int i = 0; i < 20; i++) {
        temp = [NSString stringWithFormat:@"file%d.png", i];
        data = [wrapper bundle_useFile:temp withMappedData:mData];
    }
}

- (IBAction)twentyMbRand:(id)sender
{
    
}

- (IBAction)fortyMbSeq:(id)sender
{
    NSString *temp;
    NSData *data;
    for (int i = 0; i < 40; i++) {
        temp = [NSString stringWithFormat:@"file%d.png", i];
        data = [wrapper bundle_useFile:temp withMappedData:mData];
    }
}

- (IBAction)fortyMbRand:(id)sender
{
    
}

- (IBAction)eightyMbSeq:(id)sender
{
    NSString *temp;
    NSData *data;
    for (int i = 0; i < 80; i++) {
        temp = [NSString stringWithFormat:@"file%d.png", i];
        data = [wrapper bundle_useFile:temp withMappedData:mData];
    }
}

- (IBAction)eightyMbRand:(id)sender
{
    
}

- (IBAction)oneSixtyMbSeq:(id)sender
{
    NSString *temp;
    NSData *data;
    for (int i = 0; i < 160; i++) {
        temp = [NSString stringWithFormat:@"file%d.png", i];
        data = [wrapper bundle_useFile:temp withMappedData:mData];
    }
}

- (IBAction)oneSixtyMbRand:(id)sender
{
    
}

- (IBAction)stop:(id)sender
{
    bundle_stop(mData);
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    [imageView release];
    [textField release];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

@end
