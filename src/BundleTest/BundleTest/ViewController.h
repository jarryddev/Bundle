//
//  ViewController.h
//  BundleTest
//
//  Created by Jarryd Hall on 5/17/12.
//  Copyright (c) 2012 Jarryd Hall. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

- (IBAction)loadTwenty:(id)sender;
- (IBAction)loadForty:(id)sender;
- (IBAction)loadEighty:(id)sender;
- (IBAction)loadOneSixty:(id)sender;

- (IBAction)twentyMbSeq:(id)sender;
- (IBAction)twentyMbRand:(id)sender;
- (IBAction)fortyMbSeq:(id)sender;
- (IBAction)fortyMbRand:(id)sender;
- (IBAction)eightyMbSeq:(id)sender;
- (IBAction)eightyMbRand:(id)sender;
- (IBAction)oneSixtyMbSeq:(id)sender;
- (IBAction)oneSixtyMbRand:(id)sender;

- (IBAction)stop:(id)sender;

@end
