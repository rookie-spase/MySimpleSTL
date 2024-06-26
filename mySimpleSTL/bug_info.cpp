

// -------------------修复好的bug------------------

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 致命bug2：deque在erase的时候    val.erase(val.begin()+10);  超过10就会出现重复的情况，而没有删除
#include"deque.h"
void test_deque() {
    mySimpleSTL::deque<int> val;

    for (int i = 0; i < 20; ++i) {
        val.push_back(i);
    }


    val.erase(val.begin() + 10);

    for (auto e : val)
        cout << e << " ";
}
 预期： 0到19中10没了
 实际：0 1 2 3 4 5 6 7 8 9 10 10 10 10 10 10 10 10 10
 解决: 成功解决
 解决过程： 追溯erase()的实现，将mySimpleSTL::copy改为std::copy即成功；
 原因： 我实现的mySimpleSTL算法中将copy算法写错了
 避免再次范： 在实现一个东西的时候一定要测试完全，而不是仅仅测试一点，因为这会导致后续实现会出现问题；
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  致命bug1：map的元素超过7就就会在delete报错
      总结：下次用shared_ptr吧，这个东西太费头发了
 mySimpleSTL::map<int, int> abc;
     mySimpleSTL::map<int, int> bef;

     abc.insert({1,2});
     abc.insert({11,992});
     abc.insert({12,2});
     bef.insert({ 1,2 });
     bef.insert({ 11,992 });
     bef.insert({ 12,2 });

     for (auto e : abc) {
         cout << e.second << endl;
     }
  试：我将map换成rb_tree是没有问题的，那么问题就出现在map的adapter
  解决：成功解决
  解决过程：将map重新实现一遍之后就不存在这些问题了，所以是map的问题
  避免再次范：这个存粹是有可能写出的，不存在什么再次范；
  总结：要查出错误，需要一层一层的判断，可能最底层是没问题的
           所以排查的顺序是  最底层->最高层  一层一层的排查

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */




 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   致命bug3：proirity_queue的排序不对
  测试代码，将1到100 push ，预测结果是从100到1输出，但是实际是乱序
   解决: 成功解决
  解决过程： 逐一测试代码，发现将pop函数给gpt修改好了之后，就能够运行，所以问题处在pop上
  问题原因：一样是测试不完全，有了现在的gpt，感觉是真的方便；
  避免再犯：没什么避免的，就多测试，多找bug，让gpt改就好;
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
