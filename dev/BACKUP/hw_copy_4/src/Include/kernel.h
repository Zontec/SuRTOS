#ifndef __KERNEL_H__
#define __KERNEL_H__

#define NOP()

#define INF_LOOP                            while(1)

#define PWR_GLITCH_FUNC_PROTECT(f)          if(1) f;if(0)do{volatile char a = 0; a++, a++, a++, a++, a++;}while(1)

#endif /* __KERNEL_H__ */