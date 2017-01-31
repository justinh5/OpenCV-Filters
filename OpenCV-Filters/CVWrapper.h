//
//  CVWrapper.h
//  OpenCV-Filters
//
//  Created by Justin Haupt on 1/17/17.
//  Copyright © 2017 Justin Haupt. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ObjFilters : NSObject

+ (NSString*) filter:(int)selection source:(NSString*)path_source option1:(int)param1 option2:(int)param2 option3:(int)param3;
+ (NSString*) editPath: (const char *)outPath;


@end
