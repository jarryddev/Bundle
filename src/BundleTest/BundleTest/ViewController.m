//
//  ViewController.m
//  BundleTest
//
//  Created by Jarryd Hall on 5/17/12.
//  Copyright (c) 2012 Jarryd Hall. All rights reserved.
//

#import "ViewController.h"
#import "integration.c"


@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    NSData *data;
    
    // setup bundle
    struct mappedData *mData;
    mData = malloc(sizeof(struct mappedData));
    NSString *filename = @"test1.pak";
   
    // start bundle
    bundle_start([filename UTF8String], mData);
    
    
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
