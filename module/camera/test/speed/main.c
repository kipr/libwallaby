//#include <kipr/botball.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wallaby/wallaby.h>
#include <string.h>
#include <time.h>

int main()
{
#ifdef WITH_VISION_SUPPORT
    int ret;

    printf("Camera open\n");
    camera_open_device_model_at_res(0, WHITE_2016, LOW_RES);

    printf("load config\n");
    ret = camera_load_config("1");
    if (ret == 1) printf("...success\n");

    camera_update();
    printf("Cam width x height:  %d x %d\n", get_camera_width(), get_camera_height());

    printf("Num channels = %d\n", get_channel_count());


    int i;
    double start = seconds();
    const int num_imgs = 60;
    for (i = 0; i < num_imgs; ++i)
    {
       camera_update();
       int x = get_object_count(0) > 0 ? get_object_center_x(0,0) : -1;
       int y = get_object_count(0) > 0 ? get_object_center_y(0,0) : -1;
       printf("%f objs: %d  (%d,%d)\n", seconds(), get_object_count(0), x, y);
       // printf("%f\n", seconds());
    }
    double stop = seconds();

    printf("%f fps\n", ((double)num_imgs/(stop-start)));

    printf("Camera close\n");
    camera_close();
#else
    printf("This platform does not support camera");
#endif

    return 0;
}
