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
NSData *data;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // setup bundle
    mData = malloc(sizeof(struct mappedData));
    
    
    
}
- (IBAction)loadTwenty:(id)sender
{
    NSString *filename = @"test1.pak";
    bundle_start([filename UTF8String], mData);
}

- (IBAction)loadForty:(id)sender
{
    NSString *filename = @"test1.pak";
    bundle_start([filename UTF8String], mData);
}

- (IBAction)loadEighty:(id)sender
{
    NSString *filename = @"test1.pak";
    bundle_start([filename UTF8String], mData);
}

- (IBAction)loadOneSixty:(id)sender
{
    NSString *filename = @"test1.pak";
    bundle_start([filename UTF8String], mData);
}

- (IBAction)twentyMbSeq:(id)sender
{
    
}

- (IBAction)twentyMbRand:(id)sender
{
    
}

- (IBAction)fortyMbSeq:(id)sender
{
    
}

- (IBAction)fortyMbRand:(id)sender
{
    
}

- (IBAction)eightyMbSeq:(id)sender
{
    
}

- (IBAction)eightyMbRand:(id)sender
{
    
}

- (IBAction)oneSixtyMbSeq:(id)sender
{
    
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
