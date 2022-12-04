import numpy as np

class TrajectoryGenerator:
    def __init__(self, pis, pfs, Dt, ti):
        self.pis = pis
        self.pfs = pfs
        self.ti = ti
        self.Dt = Dt
        self.polX = self.pol(pis[0], pfs[0])
        self.polY = self.pol(pis[1], pfs[1])

    def pol(self, pi, pf):
        Dt = self.Dt

        L = np.array([pi, pf, 0, 0, 0, 0])
        A = np.array([
            [0, 0, 0, 0, 0, 1], 
            [Dt**5, Dt**4, Dt**3, Dt**2, Dt, 1], 
            [0, 0, 0, 0, 1, 0], 
            [5*(Dt**4), 4*(Dt**3), 3*(Dt**2), 2*Dt, 1, 0], 
            [0, 0, 0, 2, 0, 0], 
            [20*(Dt**3), 3*(Dt**2), 6*Dt, 2, 0, 0]])

        return np.dot(np.linalg.inv(A), L)

    def p(self, t, a):
        t_shift = t - self.ti
        pos = a[0]*(t_shift**5) + a[1]*(t_shift**4) + a[2]*(t_shift**3) + a[3]*(t_shift**2) + a[4]*t_shift + a[5]
        return pos

    def dp(self, t, a):
        t_shift = t - self.ti
        pos = a[0]*5*(t_shift**4) + a[1]*4*(t_shift**3) + a[2]*3*(t_shift**2) + a[3]*2*t_shift + a[4]
        return pos

    def X(self, t):
        return np.array([self.p(t, self.polX), self.p(t, self.polY)])

    def dX(self, t):
        return np.array([self.dp(t, self.polX), self.dp(t, self.polY)])