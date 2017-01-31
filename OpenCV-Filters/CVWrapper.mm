//
//  CVWrapper.m
//  OpenCV-Filters
//
//  Created by Justin Haupt on 1/17/17.
//  Copyright © 2017 Justin Haupt. All rights reserved.
//


#import "CVWrapper.h"
#import "Filters.h"

@implementation ObjFilters



+ (NSString*) filter:(int)selection source:(NSString*)path_source option1:(int)param1 option2:(int)param2 option3:(int)param3;
{
    char const * source = [path_source UTF8String];
    NSString * output_path = nil;
    
    switch (selection) {
        case 0: {
            const char filename[] = "box_output.jpg";
            Filters::box(source, filename, param1, param2);
            output_path = [self editPath:filename];
            break;
        }
        case 1: {
            const char filename[] = "gaussian_output.jpg";
            Filters::gaussian(source, filename, param1);
            output_path = [self editPath:filename];
            break;
        }
        case 2: {
            const char filename[] = "median_output.jpg";
            Filters::median(source, filename, param1);
            output_path = [self editPath:filename];
            break;
        }
        case 3: {
            const char filename[] = "bilateral_output.jpg";
            Filters::bilateral(source, filename, param1, param2, param3);
            output_path = [self editPath:filename];
            break;
        }
        case 4: {
            const char filename[] = "canny_edge_output.jpg";
            Filters::canny_edge(source, filename, param1, param2);
            output_path = [self editPath:filename];
            break;
        }
        case 5: {
            const char filename[] = "nl_mean_output.jpg";
            Filters::non_local_mean(source, filename, param1, param2);
            output_path = [self editPath:filename];
            break;
        }
        case 6: {
            const char filename[] = "haar_wavelet_output.jpg";
            Filters::haar_wavelet_denoise(source, filename, param1);
            output_path = [self editPath:filename];
            break;
        }
        case 7: {
            const char filename[] = "anisotropic_diffusion_output.jpg";
            Filters::anisotropic_diffusion_denoise(source, filename, param1);
            output_path = [self editPath:filename];
            break;
        }
    }

    return output_path;
}


+ (NSString*) editPath: (const char *)outPath
{
    NSString * path = [NSString stringWithCString:outPath encoding:NSASCIIStringEncoding];
    return path;
}



@end
