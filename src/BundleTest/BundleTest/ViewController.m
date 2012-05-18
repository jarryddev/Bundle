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

struct mappedData *mData;
Bundle_CocoaWrapper *wrapper;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // setup bundle
    mData = malloc(sizeof(struct mappedData));
    
    wrapper = [[Bundle_CocoaWrapper alloc] init];
    
}

- (IBAction)loadTwenty:(id)sender
{
    NSString *filename = @"test20.pak";
    bundle_start([filename UTF8String], mData);
}

- (IBAction)loadForty:(id)sender
{
    NSString *filename = @"test40.pak";
    bundle_start([filename UTF8String], mData);
}

- (IBAction)loadEighty:(id)sender
{
    NSString *filename = @"test80.pak";
    bundle_start([filename UTF8String], mData);
}

- (IBAction)loadOneSixty:(id)sender
{
    NSString *filename = @"test160.pak";
    bundle_start([filename UTF8String], mData);
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
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

@end
