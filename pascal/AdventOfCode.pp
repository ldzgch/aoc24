program AdventOfCode;

type
    PInt64 = ^Int64;
    DynArray =
        record
            data: ^Int64;
            len, cap: Int32;
        end;
    Int32 = LongInt;
// const

procedure Swap(a, b: PInt64);
var tmp: Int64;
begin
    tmp := b^;
    b^ := a^;
    a^ := tmp;
end;

// inplace quicksort
// end points one off the last element
procedure Sort(pstart, pend: PInt64);
var
    pivot: Int64;
    left, right: PInt64;
    len: Int32;
begin
    len := pend - pstart;
    if len < 2 then exit;
    if len = 2 then
    begin
        if pstart^ > (pstart+1)^ then
            Swap(pstart, pstart+1);
        exit;
    end;
    pivot := pstart^;
    left := pstart + 1; right := pend - 1;
    while left < right do
    begin
        if left^ > pivot then
        begin
            while (right^ > pivot) and (right > left) do
                dec(right);
            Swap(left, right);
        end
        else inc(left);
    end;
    if pstart^ > left^ then
        Swap(pstart, left);
    Sort(pstart, left);
    Sort(left, pend);
end;


function DynArrayMake(size: Int32): DynArray;
var
    ptr: ^Int64;
begin
    ptr := GetMem(sizeof(Int64) * size);
    DynArrayMake.data := ptr;
    DynArrayMake.len := 0;
    DynArrayMake.cap := size;
end;

procedure DynArrayAppend(VAR a: DynArray; value: Int64);
var
    new_ptr: ^Int64;
    i : Int32;
begin
    if a.len = a.cap then
    begin
        a.cap := a.cap * 2;
        new_ptr := GetMem(sizeof(Int64) * a.cap);
        for i := 0 to (a.len - 1) do
            new_ptr[i] := a.data[i];
        FreeMem(a.data);
        a.data := new_ptr;
    end;

    a.data[a.len] := value;
    a.len := a.len + 1;
end;

function DynArrayAt(VAR a: DynArray; index: Int32) : Int64;
begin
    Assert((index < a.len) and (index >= 0));
    DynArrayAt := a.data[index];
end;

procedure Open(VAR f: text; path: string);
begin
    assign(f, path);
    reset(f);
end;

procedure Day1(part1: boolean);
var
    DataFile: text;
    num1, num2: Int64;
    list1, list2: DynArray;
    CountArray: DynArray;
    idx: Int32;
    sum: Int64;
begin
    open(DataFile, '../data/data1.txt');
    list1 := DynArrayMake(8);
    list2 := DynArrayMake(8);

    while not eof(DataFile) do
    begin
        readln(DataFile, num1, num2);
        DynArrayAppend(list1, num1);
        DynArrayAppend(list2, num2);
    end;
    close(DataFile);
    if part1 then
    begin
        Sort(list1.data, list1.data+list1.len);
        Sort(list2.data, list2.data+list2.len);
        sum := 0;
        for idx := 0 to list1.len - 1 do
        begin
            sum += abs(list1.data[idx] - list2.data[idx]);
        end;
        writeln(sum);
    end
    else
    begin
        CountArray := DynArrayMake(100000);
        for idx := 0 to 100000 do
            CountArray.data[idx] := 0;
        for idx := 0 to list2.len do
            CountArray.data[list2.data[idx]] += 1;
        sum := 0;
        for idx := 0 to list1.len do
            sum += list1.data[idx] * CountArray.data[list1.data[idx]];
        writeln(sum);
    end;
end;

procedure Day2(part1: boolean);
var
    DataFile: text;
    row, diff: DynArray;
    num: Int64;
    dummy_char:char;
    sum_safe: Int64;
    skip, idx: Int32;

    function IsSafe(diff: DynArray): boolean;
    var
        IsIncreasing, IsDecreasing, IsSmooth: boolean;
        idx: Int32;
    begin
        IsIncreasing := true;
        for idx := 0 to diff.len - 1 do
            IsIncreasing := IsIncreasing and (DynArrayAt(diff, idx) > 0);

        IsDecreasing := true;
        for idx := 0 to diff.len - 1 do
            IsDecreasing := IsDecreasing and (DynArrayAt(diff, idx) < 0);

        IsSmooth := true;
        for idx := 0 to diff.len - 1 do
            IsSmooth := IsSmooth and
                (abs(DynArrayAt(diff, idx)) <= 3) and
                (DynArrayAt(diff, idx) <> 0);
        IsSafe := (IsIncreasing or IsDecreasing) and IsSmooth;
    end;
begin
    open(DataFile, '../data/data2.txt');
    sum_safe := 0;
    row := DynArrayMake(8);
    diff := DynArrayMake(8);
    while not eof(DataFile) do
    begin
        row.len := 0;
        diff.len := 0;
        while not eoln(DataFile) do
        begin
            read(DataFile, num);
            DynArrayAppend(row, num);
            // writeln(row.len, ':', row.data[row.len-1]);
        end;
        read(DataFile, dummy_char);

        if part1 then
        begin
            for idx := 0 to row.len - 2 do
            begin
                DynArrayAppend(diff, row.data[idx+1] - row.data[idx]);
            end;
            if IsSafe(diff) then sum_safe += 1;
        end
        else
        begin
            for skip := 0 to row.len - 1 do
            begin
                diff.len := 0;
                for idx := 0 to skip - 1 do
                begin
                    DynArrayAppend(diff, row.data[idx+1] - row.data[idx]);
                end;
                for idx := skip+1 to row.len - 2 do
                begin
                    DynArrayAppend(diff, row.data[idx+1] - row.data[idx]);
                end;
                writeln(row.len - diff.len);
                if IsSafe(diff) then
                begin
                    sum_safe += 1;
                    break;
                end;
            end;
        end;
    end;
    writeln(sum_safe);
end;

begin
    if paramcount() < 1 then
    begin
        writeln('Provide exercise number');
        exit;
    end;
    case paramstr(1) of
    '1':Day1(true);
    '1.5':Day1(false);
    '2':Day2(true);
    '2.5':Day2(false);
    end;
end.
