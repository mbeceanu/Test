def longest_strictly_increasing_subsequence(nums):
    # node_lst=[]
    class node():
        def __init__(self, val=None, ct=1, left=None, right=None, height=1):
            self.val=val
            self.ct=ct
            self.max_left=ct-1
            self.left=left
            self.right=right
            self.height=height
            # node_lst+=[self]

        # def copy(self):
        #     return node(self.val, self.ct, self.left, self.right, self.height)

        def bal(self):
            if self.left==None:
                left_height=0
            else:
                left_height=self.left.height
            if self.right==None:
                right_height=0
            else:
                right_height=self.right.height
            return right_height-left_height

        def h(self):
            if self.left==None:
                left_height=0
            else:
                left_height=self.left.height
            if self.right==None:
                right_height=0
            else:
                right_height=self.right.height
            return max(left_height, right_height)+1

        # def print_tree(self):
        #     if self.left!=None:
        #         print "l(",
        #         self.left.print_tree()
        #         print ")l",
        #     print (self.val, self.ct, self.max_left),
        #     if self.right!=None:
        #         print "r(",
        #         self.right.print_tree()
        #         print ")r",

        # def print_top_tree(self):
        #     print "begin_tree",
        #     self.print_tree()
        #     print "end_tree"

        def max_val(self):
            ans=0
            while self!=None:
                if max(self.ct, self.max_left)>ans:
                    ans=max(self.ct, self.max_left)
                self=self.right
            return ans

        # def max_val2(self):
        #     if self.left!=None:
        #         max_left=self.left.max_val2()
        #     else:
        #         max_left=0
        #     if self.right!=None:
        #         max_right=self.right.max_val2()
        #     else:
        #         max_right=0
        #     return max(max_left, max_right, self.ct)

        def rotate_left(self):
            right=self.right
            right.left, self.right=self, right.left
            self.height=self.h()
            right.height=right.h()
            return right

        def rotate_right(self):
            left=self.left
            left.right, self.left=self, left.right
            self.height=self.h()
            left.height=left.h()
            return left

        def rebalance_right(self):
            right=self.right
            if right.bal()>=0:
                return self.rotate_left()
            else:
                self.right=right.rotate_right()
                return self.rotate_left()

        def rebalance_left(self):
            left=self.left
            if left.bal()<=0:
                return self.rotate_right()
            else:
                self.left=left.rotate_left()
                return self.rotate_right()             

        def add_node(self, val, ct=1):
            if val<self.val:
                if self.left==None:
                    self.left=node(val, ct)
                    # self.bal-=1
                    if self.right==None:
                        self.height=2#self.h()
                    max_left=max(ct, self.max_left)
                    self.max_left=max_left
                else:
                    old_height=self.left.height
                    self.left, max_left=self.left.add_node(val, ct)
                    max_left=max(max_left, self.max_left)
                    self.max_left=max_left
                    if self.left.height>old_height:
                        self.height=self.h()
                        if self.bal()<=-2:
                            self=self.rebalance_left()
                return self, max_left
            elif val>self.val:
                ct=max(ct, self.ct+1, self.max_left+1)
                if self.right==None:
                    self.right=node(val, ct)
                    # self.bal+=1
                    if self.left==None:
                        self.height=2#self.h()
                    return self, ct
                else:
                    old_height=self.right.height
                    self.right, max_left=self.right.add_node(val, ct)
                    if self.right.height>old_height:
                        self.height=self.h()
                        if self.bal()>=2:
                            self=self.rebalance_right()
                    return self, max_left
            else:
                self.ct=max(ct, self.ct, self.max_left+1)
                self.max_left=max(ct-1, self.max_left)
                return self, self.max_left+1

    head=node(nums[0])

    for num in nums:
        head, max_left=head.add_node(num)
        # head.print_top_tree()
    return head.max_val()
