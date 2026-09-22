package basics

import "core:fmt"

sum :: proc(n: int) -> int {
    fmt.println("Number was: ", n)

    res := 0
    for i in 0..<n {
        fmt.println(i)
        res += i
    }

    return res
}

ifstatements :: proc() {
    alvero :: 12
    if alvero == 12 {
        fmt.println(alvero)
    }
}

pointer :: proc(user: ^User) {
    user.age = 23
}

structs :: proc() -> User {
    user1: User
    user1.age = 12
    user1.name = "Herbert"

    fmt.println(user1)

    user2: User = {name = "Gilbert", age = 33}

    fmt.println(user2)

    pointer(&user1)

    return user1
}

User :: struct {
    name: string,
    age: int,
}

main :: proc() {
    fmt.println("Hello, World!")

    number: int

    number = 2

    floating : f32 : 23.4

    fmt.println("Number: ", number)

    fmt.println("Sum: ", sum(number))
    
}