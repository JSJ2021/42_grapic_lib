Checking the linked article and extracting the functions mentioned.
MiniLibX 공식 문서(https://harm-smits.github.io/42docs/libs/minilibx/)를 바탕으로 함수별 사용법, 매개변수, 반환값, 동작 원리를 정리했습니다.

## MiniLibX 함수 정리 (harm-smits 문서 요약)

### 1. 초기화

#### `mlx_init`
```c
void *mlx_init(void);
```
- 설명: MiniLibX 인스턴스를 초기화하고 디스플레이와 연결합니다. 다른 모든 mlx 호출 전에 반드시 호출해야 합니다.
- 매개변수: 없음
- 반환값:
  - `void *`: 성공 시 mlx 인스턴스 포인터(이후 `mlx_ptr`로 사용)
  - 실패 시: `NULL`

---

### 2. 윈도우 관리

#### `mlx_new_window`
```c
void *mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title);
```
- 설명: 새로운 윈도우를 생성하고 윈도우 인스턴스 포인터를 반환합니다.
- 매개변수:
  - `mlx_ptr`: `mlx_init()`로 얻은 인스턴스
  - `size_x`, `size_y`: 윈도우 크기(픽셀)
  - `title`: 윈도우 제목 문자열
- 반환값:
  - `void *`: 성공 시 윈도우 포인터(`win_ptr`)
  - 실패 시: `NULL`

#### `mlx_clear_window`
```c
int mlx_clear_window(void *mlx_ptr, void *win_ptr);
```
- 설명: 주어진 윈도우를 비웁니다(검정색으로 초기화). 문서에서는 추천되지 않으며, 대신 이미지를 재사용해 `mlx_put_image_to_window`로 갱신하는 것을 권장합니다.
- 매개변수: `mlx_ptr`, `win_ptr`
- 반환값: 문서상 "has no return value (bc)"로 표기되어 실제로는 의미 있는 값을 제공하지 않습니다(관례상 0 또는 무시 가능).

#### `mlx_destroy_window`
```c
int mlx_destroy_window(void *mlx_ptr, void *win_ptr);
```
- 설명: 지정된 윈도우를 파괴합니다.
- 매개변수: `mlx_ptr`, `win_ptr`
- 반환값: 문서상 의미있는 값 없음(무시 가능).
- 주의: `mlx_ptr` 자체를 안전하게 해제하는 표준 API는 제공되지 않으므로 프로그램 종료 시 `exit()`로 마무리하는 방식이 일반적입니다.

---

### 3. 픽셀/문자열 출력

#### `mlx_pixel_put`
```c
int mlx_pixel_put(void *mlx_ptr, void *win_ptr, int x, int y, int color);
```
- 설명: 윈도우의 (x,y) 위치에 단일 픽셀을 즉시 그립니다. 내부적으로 출력 잠금/갱신이 발생하므로 느립니다.
- 매개변수:
  - `x`, `y`: 좌표 (왼쪽 위가 (0,0))
  - `color`: 0xTTRRGGBB 형식(상위 바이트는 투명도 등 하드웨어에 따라 사용)
- 반환값: 문서상 의미 있는 값 없음(무시 가능).
- 권장 사용법: 많은 픽셀을 그릴 경우 `mlx_new_image`로 버퍼에 그린 뒤 `mlx_put_image_to_window`로 한 번에 출력하세요.

#### `mlx_string_put`
```c
int mlx_string_put(void *mlx_ptr, void *win_ptr, int x, int y, int color, char *string);
```
- 설명: 윈도우에 문자열을 출력합니다(폰트/크기 제어는 불가).
- 매개변수: `x`, `y`, `color`, `string`
- 반환값: 문서상 의미 있는 값 없음(무시 가능).

---

### 4. 이미지(버퍼) 관련 함수

#### `mlx_new_image`
```c
void *mlx_new_image(void *mlx_ptr, int width, int height);
```
- 설명: MLX 호환 이미지를 메모리에 생성(버퍼). 화면에 그릴 모든 픽셀을 이 이미지에 작성 후 한 번에 출력하는 방식이 권장됩니다.
- 매개변수: `mlx_ptr`, `width`, `height`
- 반환값: 성공 시 이미지 포인터(`img_ptr`), 실패 시 `NULL`.

#### `mlx_get_data_addr`
```c
char *mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian);
```
- 설명: 이미지의 픽셀 데이터가 메모리에 어디에 있는지와 메타정보들을 반환합니다.
- 매개변수:
  - `img_ptr`: `mlx_new_image`로 얻은 이미지 포인터
  - `bits_per_pixel`: (출력 인자) 픽셀 당 비트 수
  - `size_line`: (출력 인자) 한 줄의 바이트 수 (메모리 상의 stride)
  - `endian`: (출력 인자) 엔디언(0 = little, 1 = big)
- 반환값: `char *` — 이미지 데이터의 시작 주소
- 동작 원리 요약: 1차원 메모리 배열에 픽셀이 나열되어 있으므로, 좌표 (x,y)의 바이트 오프셋은 `y * size_line + x * (bits_per_pixel / 8)`로 계산합니다. 픽셀을 쓰려면 적절히 포인터를 형변환한 뒤(예: `(unsigned int *)`) 색값을 대입합니다.

예시: 32bpp, little endian인 경우
```c
int bpp, line, endian;
char *data = mlx_get_data_addr(img, &bpp, &line, &endian);
int pos = y * line + x * (bpp / 8);
*(unsigned int *)(data + pos) = color;
```

#### `mlx_put_image_to_window`
```c
int mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y);
```
- 설명: 이미지 버퍼를 윈도우의 (x,y) 위치에 출력합니다. 많은 픽셀을 한 번에 갱신할 때 사용.
- 매개변수: `mlx_ptr`, `win_ptr`, `img_ptr`, `x`, `y`
- 반환값: 문서상 의미 있는 값 없음(무시 가능).
- 주의: 이미지 메모리를 변경하면 즉시 윈도우에 반영될 수 있으므로, 동기화에 주의하세요.

#### `mlx_destroy_image`
```c
int mlx_destroy_image(void *mlx_ptr, void *img_ptr);
```
- 설명: 이미지 리소스 해제
- 매개변수: `mlx_ptr`, `img_ptr`
- 반환값: 문서상 의미 있는 값 없음(무시 가능).

---

### 5. 이미지 파일/데이터 변환

#### `mlx_xpm_to_image`
```c
void *mlx_xpm_to_image(void *mlx_ptr, char **xpm_data, int *width, int *height);
```
- 설명: 메모리상의 XPM 데이터 배열로부터 이미지 생성
- 매개변수: `mlx_ptr`, `xpm_data`, `width`, `height`(출력)
- 반환값: 성공 시 `img_ptr`, 실패 시 `NULL`.

#### `mlx_xpm_file_to_image`
```c
void *mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width, int *height);
```
- 설명: XPM 파일을 읽어 이미지 생성
- 반환값: 성공 시 `img_ptr`, 실패 시 `NULL`.

#### `mlx_png_file_to_image`
```c
void *mlx_png_file_to_image(void *mlx_ptr, char *filename, int *width, int *height);
```
- 설명: PNG 파일을 이미지로 변환
- 주의: 문서에서는 기존 구현에서 메모리 누수 이슈를 지적하며 XPM 사용을 권장합니다.

---

### 6. 이벤트 / 훅 / 루프

#### `mlx_mouse_hook`
```c
int mlx_mouse_hook(void *win_ptr, int (*f)(), void *param);
```
- 설명: 마우스 클릭 이벤트 훅 등록
- 매개변수: `win_ptr`, `f`(콜백), `param`
- 콜백 시그니처: `int f(int button, int x, int y, void *param)`
- 주의: 문서상 일부 구현에서는 마우스 이벤트가 제한적일 수 있다고 언급합니다.

#### `mlx_key_hook`
```c
int mlx_key_hook(void *win_ptr, int (*f)(), void *param);
```
- 설명: 키 입력 훅 등록
- 콜백 시그니처: `int f(int keycode, void *param)`

#### `mlx_expose_hook`
```c
int mlx_expose_hook(void *win_ptr, int (*f)(), void *param);
```
- 설명: 윈도우 노출(expose) 이벤트(갱신 필요 시) 훅 등록
- 콜백 시그니처: `int f(void *param)` (문서상 동작 미정인 경우도 있음)

#### `mlx_loop_hook`
```c
int mlx_loop_hook(void *mlx_ptr, int (*f)(), void *param);
```
- 설명: 이벤트가 없을 때에도 반복적으로 호출되는 콜백 등록 (게임 루프 등)
- 콜백 시그니처: `int f(void *param)`

#### `mlx_loop`
```c
int mlx_loop(void *mlx_ptr);
```
- 설명: 등록된 훅들을 순서대로 호출하며 이벤트 루프를 실행합니다(대개 프로그램이 여기서 블록됨).
- 매개변수: `mlx_ptr`
- 반환값: 문서상 의미 있는 값 없음(무시 가능).

---

### 7. 마우스 관련 유틸

`mlx_mouse_hide`, `mlx_mouse_show`, `mlx_mouse_move`, `mlx_mouse_get_pos` 등은 각각 마우스 숨김/표시/이동/위치 조회 기능을 제공합니다. 대부분 매개변수와 반환값은 문서에 명시되어 있으나 반환값은 의미 있는 값이 없는 경우가 많습니다(무시 가능).

예: `mlx_mouse_get_pos(void *win_ptr, int *x, int *y)` — 현재 윈도우에서의 마우스 좌표를 `x`, `y`에 씁니다.

---

### 8. 키 자동 반복 및 동기화

`mlx_do_key_autorepeatoff(void *mlx_ptr)` / `mlx_do_key_autorepeaton(void *mlx_ptr)` — 키 자동 반복 끄기/켜기

`mlx_do_sync(void *mlx_ptr)` — 모든 윈도우 프레임 동기화 (문서상 동작 호출용).

---

### 9. 화면 크기 조회

#### `mlx_get_screen_size`
```c
int mlx_get_screen_size(void *mlx_ptr, int *sizex, int *sizey);
```
- 설명: 현재 화면(모니터) 해상도(픽셀)를 얻어옵니다.
- 반환값: 문서상 의미 있는 값 없음; `sizex`, `sizey`에 결과를 씁니다.

---

### 반환값 관련 요약(문서상 관찰)
- harm-smits 문서에서는 많은 `int` 반환 함수에 대해 "has no return value (bc)"라고 표기되어 있습니다. 이는 구현상 호출자가 반환값을 신경 쓰지 않아도 동작하지만, 라이브러리/플랫폼/버전마다 동작이 다를 수 있으므로 다음 권장사항을 따르세요:
  - `void *`을 반환하는 생성 함수(예: `mlx_init`, `mlx_new_window`, `mlx_new_image`, 이미지 로딩 함수 등)는 실패 시 `NULL`을 반환합니다. 이들은 반드시 실패 체크를 하세요.
  - `int`를 반환하는 함수들 중 다수는 문서상 의미 있는 반환값을 제공하지 않지만(관례상 0 반환), 플랫폼별 다른 구현에서는 에러 코드를 반환할 수 있습니다. 안전을 위해 중요한 리소스 해제/생성에 대해 에러 상황을 가정해 예외 처리를 하세요.

---

필요하시면 이 파일을 기반으로 각 함수별 간단 사용 예제(코드 스니펫)를 추가하거나, 직접 테스트해 반환 동작을 확인하는 스크립트를 만들어 드리겠습니다.