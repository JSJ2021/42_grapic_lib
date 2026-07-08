#include "mlx/mlx.h"

// 이미지 버퍼 정보를 담는 구조체
// img: 이미지 객체 포인터, addr: 픽셀 데이터 포인터,
// bits_per_pixel: 픽셀당 비트 수, line_length: 한 줄의 바이트 수, endian: 바이트 순서
typedef struct  s_data
{
  void	*img;
  char	*addr;
  int	bits_per_pixel;
  int	line_length;
  int	endian;
}               t_data;

// 주어진 좌표에 색상을 그리는 함수
// data->addr에서 해당 위치의 메모리 주소를 계산한 뒤 픽셀을 기록한다.
void  my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
  char *dst;

  dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
  *(unsigned int*)dst = color;
}

int main(void)
{
    void    *mlx;
    void    *mlx_win;
    t_data  img;
    int     i;
    int     j;
    int     k;

    // MiniLibX 초기화
    mlx = mlx_init();

    // 윈도우 생성: 너비 600, 높이 400, 제목 "miniRT"
    mlx_win = mlx_new_window(mlx, 600, 400, "miniRT");

    // 이미지 버퍼 생성
    img.img = mlx_new_image(mlx, 600, 400);

    // 이미지 데이터 주소를 얻어와서 픽셀 쓰기 준비
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

    // 왼쪽 삼각형 모양의 픽셀을 그리는 반복문
    // i는 x축, k는 y축 시작 위치를 조절하는 변수
    for (i = 250, k = 99; i < 300; i++, k -= 2)
    {
        for (j = 150 + k; j < 250; j++)
            my_mlx_pixel_put(&img, i, j, 0x0000FF00);
    }

    // 오른쪽 삼각형 모양의 픽셀을 그리는 반복문
    for (i = 300, k = 0; i < 350; i++, k += 2)
    {
      for (j = 150 + k; j < 250; j++)
        my_mlx_pixel_put(&img, i, j, 0x0000FF00);
    }

    // 생성한 이미지 버퍼를 윈도우에 표시
    mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
    mlx_clear_window(mlx, mlx_win);
    // 이벤트 루프를 시작하여 창이 닫히기 전까지 실행 유지
    mlx_loop(mlx);
    return (0);
}