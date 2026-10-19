global TwoSum

section .text
;
;    int* nums, int numsLen, int* targets, int targetsLen
;    rdi        esi          rdx           ecx
;

TwoSum:
    mov eax, esi
    inc eax         ;n+1

    imul eax, esi   ;n * (n+1)
    shr eax, 1      ;n * (n+1) / 2
    shl eax, 2      ;mul by sizeof(int)

    movsxd rax, eax
    sub rsp, rax    ;reserve stack space

    xor eax, eax    ; i
    xor ebx, ebx    ; j

    xor r10d, r10d    ;sumArray index

; Calculate all possible sums
loop_over_i:
    mov r8d, [rdi + 4*eax]  ;first value

loop_over_j:
    mov ebx, eax            ; j = i

    mov r9d, [rdi + 4*ebx]  ;second value

    add r9d, r8d            ;calculate sum
    mov [rsp + 4*r10d], r9d ;arrPush
    inc r10d

    cmp ebx, esi
    inc ebx
    jl loop_over_j

    cmp eax, esi
    inc eax
    jl loop_over_i
    
    ret

